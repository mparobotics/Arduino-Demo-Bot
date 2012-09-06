#include <Servo.h>
#include <Ethernet.h>
#include <EthernetUDP.h>
#include <SPI.h>
#include <mecanumdrive.h>
#include <drive.h>
#include <communications.h>
Servo motors[4];
void setup() {
    motors[0].attach(5);
    motors[1].attach(6);
    motors[2].attach(7);
    motors[3].attach(8);
    netSetup();
    Serial.begin(9600);
}
void loop() {
  long watch = millis();
   packet in = readPacket();

   //Serial.println(in.data);
   Serial.println("joysticks:");
   Serial.println(getLeftJoy(in).x);
   Serial.println(getLeftJoy(in).y);
   //Serial.println(getRightJoy(in).x);
   drivetrain test = simpleMecanum(getLeftJoy(in), getRightJoy(in).x, &joyScaleAbs);
   delay(100);
   
   /*
   drivetrain test =  simpleMecanum(vec(0.5,0.0), 0.0, &joyScaleAbs); */
   //simpleServoDrive(drivetrainToServo(test), motors);
}
