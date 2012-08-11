/* Keep track of gyro angle over time
 * Connect Gyro to Analog Pin 0
 *
 * Sketch by eric barch / ericbarch.com
 * v. 0.1 - simple serial output
 *
 */

int gyroPin = 1;  //Gyro is connected to analog pin 0
int tempPin = 0;
float gyroVoltage = 5;         //Gyro is running at 5V
float gyroZeroVoltage = 2.501;   //Gyro is zeroed at 2.5V
float gyroSensitivity = .007;  //Our example gyro is 7mV/deg/sec
float rotationThreshold = 1;   //Minimum deg/sec to keep track of - helps with gyro drifting

float currentAngle = 0;          //Keep track of our current angle
int ledPin = 9;

void setup() {
  Serial.begin (9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  //This line converts the 0-1023 signal to 0-5V
  float gyroRate = (analogRead(gyroPin) * gyroVoltage) / 1023;
  
  float tempVolt = (analogRead(tempPin) * 5) / 1023;
  float temperature = ((tempVolt - 2.0) * 111.1111111) + 25.555556;
  

  //This line finds the voltage offset from sitting still
  gyroRate -= gyroZeroVoltage;

  //This line divides the voltage we found by the gyro's sensitivity
  gyroRate /= gyroSensitivity;

  //Ignore the gyro if our angular velocity does not meet our threshold
  if (gyroRate >= rotationThreshold || gyroRate <= -rotationThreshold) {
    //This line divides the value by 100 since we are running in a 10ms loop (1000ms/10ms)
    gyroRate /= 100;
    currentAngle += gyroRate;
  }

  //Keep our angle between 0-359 degrees
  if (currentAngle <= 0)
    currentAngle += 360;
  else if (currentAngle >= 360)
    currentAngle -= 360;

  //DEBUG
  Serial.println(currentAngle);
  digitalWrite(ledPin, HIGH);
  delay(360 / currentAngle);
  digitalWrite(ledPin, LOW);
  delay(360/ currentAngle);

} 
