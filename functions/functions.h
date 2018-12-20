int disCheck(int sensor = 1)
{

  if (sensor == 1)
  {
    return (sonar1.ping() / US_ROUNDTRIP_CM); //front
  }
  else if (sensor == 2)
  {
    return (sonar2.ping() / US_ROUNDTRIP_CM); //right
  }
  else if (sensor == 3)
  {
    return (sonar3.ping() / US_ROUNDTRIP_CM); //left
  }
  else
  {
    return 0;
  }
}

void turnz(char t, boolean enable = true)
{
  if (t == 'R')
  {
    //turn right
    digitalWrite(IN8, HIGH);
    digitalWrite(IN7, LOW);
    digitalWrite(IN5, HIGH);
    digitalWrite(IN6, LOW);
    delay(calibrateTurn);
  }
  else if (t == 'L')
  {
    //turn left
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
    digitalWrite(IN7, HIGH);
    digitalWrite(IN8, LOW);
    delay(calibrateTurn);
  }
}

void sturn360()
{
  digitalWrite(IN8, HIGH);
  digitalWrite(IN7, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  delay(1500);
  digitalWrite(IN8, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
}

void go()
{
  digitalWrite(IN6, HIGH);
  digitalWrite(IN8, HIGH);
  digitalWrite(IN7, LOW);
  digitalWrite(IN5, LOW);
}

void back()
{

  while ((disCheck(1) < (front_min_distance) / 2) && (disCheck(1) > 0) || (disCheck(2) < (side_min_distance) / 2) && (disCheck(1) > 0) || (disCheck(3) < (side_min_distance) / 2) && (disCheck(1) > 0))
  {
    digitalWrite(IN7, HIGH);
    digitalWrite(IN5, HIGH);
    digitalWrite(IN6, LOW);
    digitalWrite(IN8, LOW);
    delay(100);
  }
  digitalWrite(IN7, LOW);
  digitalWrite(IN5, LOW);
}

void stop_car()
{
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);
}

void calibrate()
{
  //Serial.print("OK: ");
  analogWrite(enB, SPEED);
  int left = map(analogRead(A4), 0, 1023, 0, 255);  //left
  int right = map(analogRead(A5), 0, 1023, 0, 255); //right

  analogWrite(enA, (left > right ? left - (left - right) : (left < right ? (left + (right - left)) : left)));

  Serial.print("right: ");
  Serial.println(right);
  Serial.print("left: ");
  Serial.println(left);
}