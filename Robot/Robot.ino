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
char packettimer = 0; //An arbitrary clock that verifies that we have activity
char violationcount = 0; //A counter of violations of the clock
drivetrain test = simpleMecanum(vec(0.0,0.0),0.0,&joyScaleAbs);
void loop() {
  long watch = millis();
   packet in = readPacket();
   Serial.println(millis() - watch);
   if (packettimer != in.data[4]) {
     violationcount = 0;
   }
   else {
     violationcount++;
   }
   if (violationcount > 30) {
     //Really, something to terminate the execution should go here, but for testing
     violationcount = 30;
   }
   if ((millis() - watch) < 20 && violationcount < 3) {
     //Serial.println(in.data);
     //Serial.println("joysticks:");
     //Serial.println(getLeftJoy(in).x);
     //Serial.println(getLeftJoy(in).y);
     //Serial.println(getRightJoy(in).x);
     vector2d leftjoy = joyScaleSquareCircle(getLeftJoy(in));
     test = simpleMecanum(leftjoy, 0.0, &joyScaleAbs);
     Serial.print(test.wheel[0]); Serial.print(" ,");
     Serial.print(test.wheel[1]); Serial.print(" ,");
     Serial.print(test.wheel[2]); Serial.print(" ,");
     Serial.print(test.wheel[3]); Serial.println(" ");
   }
   else {
       test = simpleMecanum(vec(0.0,0.0),0.0,&joyScaleAbs);
   }
   packettimer = in.data[4];
   delay(100);
   
   /*
   drivetrain test =  simpleMecanum(vec(0.5,0.0), 0.0, &joyScaleAbs); */
   //simpleServoDrive(drivetrainToServo(test), motors);
}
