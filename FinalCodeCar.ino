#include <NewPing.h>
#include <SPI.h>
#include <RF24.h>

//https://www.arduino.cc/en/Tutorial/ReadAnalogVoltage
//https://www.arduino.cc/reference/en/language/functions/math/map/
//https://www.instructables.com/id/L298n-Arduino-Library/
//https://www.instructables.com/id/Obstacle-Detection-Robot-Using-Three-Ultrasonic-Se/
////////SENSOR 1 PINS//////////
#define SENSOR_1_PIN A3
////////SENSOR 2 PINS//////////
#define SENSOR_2_PIN A2
////////SENSOR 3 PINS//////////
#define SENSOR_3_PIN A0
#define MAX_DISTANCE 150

#define front_min_distance 20
#define side_min_distance 15
int calibrateTurn = 20; //ms

#define SPEED 150

NewPing sonar1(SENSOR_1_PIN, SENSOR_1_PIN, MAX_DISTANCE);
NewPing sonar2(SENSOR_2_PIN, SENSOR_2_PIN, MAX_DISTANCE);
NewPing sonar3(SENSOR_3_PIN, SENSOR_3_PIN, MAX_DISTANCE);

const char text[32];
byte rxAddr[6] = "00001";
#define enA 6 //3 // left
int IN5 = 4;
int IN6 = 3; //6
int IN7 = 7;
int IN8 = 8;
#define enB 5 // right
int opFront, opRight, opLeft, d;

RF24 radio(9, 10);
#include <functions/functions.h>

void setup()
{

    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(IN5, OUTPUT);
    pinMode(IN6, OUTPUT);
    pinMode(IN7, OUTPUT);
    pinMode(IN8, OUTPUT);

    //=====Rithmiseis gia transmit
    radio.begin();
    radio.setRetries(15, 15);
    radio.openWritingPipe(rxAddr);
    // radio.enableDynamicPayloads();
    radio.stopListening();
    //=====Telos Rithmisewn gia transmit
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);

    analogWrite(enA, SPEED);
    analogWrite(enB, SPEED);
}

void loop()
{

    calibrate();

    //sonar1 => koitaei mprwsta / sonar2 => koitaei deksia / sonar3 => koitaei aristera

    opFront = disCheck(1);
    opRight = disCheck(2);
    opLeft = disCheck(3);

    calibrate();
    go();
    if (opFront < front_min_distance && opFront > 0)
    {

        stop_car();
        delay(1000);
        back();
        opRight = disCheck(2);

        opLeft = disCheck(3);
        delay(1000);
        if (opLeft < side_min_distance && opLeft > 0)
        {
            while (opFront < front_min_distance && opFront > 0)
            {
                turnz('R');
                opFront = disCheck(1);
            }
        }
        else if (opRight < side_min_distance && opRight > 0)
        {
            while (opFront < front_min_distance && opFront > 0)
            {
                turnz('L');
                opFront = disCheck(1);
            }
        }
        else if ((opLeft < side_min_distance && opLeft > 0) && (opRight < side_min_distance && opRight > 0))
        {
            sturn360();
        }
        else
        {
            char select = ((random(1, 3)) == 1 ? 'R' : 'L');
            while (opFront < front_min_distance && opFront > 0)
            {
                turnz(select);
                opFront = disCheck(1);
            }
        }
        delay(100);
        stop_car();
        delay(500);
    }
    else if (opLeft < side_min_distance && opLeft > 0)
    {
        stop_car();
        while (opLeft < side_min_distance && opLeft > 0)
        {
            turnz('R');
            opLeft = disCheck(3);
        }
        stop_car();
        delay(500);
    }
    else if (opRight < side_min_distance && opRight > 0)
    {
        stop_car();
        while (opRight < side_min_distance && opRight > 0)
        {
            turnz('L');
            opRight = disCheck(2);
        }
        stop_car();
        delay(500);
    }
    calibrate();

    //======== gia apostolh twn data//=======
    const char text[50];
    sprintf(text, "%d,%d,%d", opFront, opRight, opLeft);
    radio.write(&text, sizeof(text));
    //Serial.println(text);
    radio.flush_tx();
    //=======
}