#include <Servo.h>
#include <Ethernet.h>
#include <EthernetUDP.h>
#include <SPI.h>
#include <mecanumdrive.h>
#include <joyconvert.h>
#include <drive.h>
#include <communications.h>
//#include <gyro.h>
Servo motors[4];
void setup() {
    motors[0].attach(7);
    motors[1].attach(6);
    motors[2].attach(5);
    motors[3].attach(8);
    netSetup();
    //Serial.begin(9600);
    //Serial.println("You're a sill");
}

void shutdown() {
     int i;
     for (i=0; i<18; i++){
       //detatchInterrupt(i);
     }
     while (1 == 1) {
     }
}

//scalar_t rot = 0.0; // the rotation of the robot
char packettimer = 0; //An arbitrary clock that verifies that we have activity
char violationcount = 0; //A counter of violations of the clock
char testTimer = 0;
drivetrain test = simpleMecanum(vec(0.0,0.0),0.0,&joyScaleAbs);
void loop() {
  long watch = millis();
   packet in = readPacket();
   //Serial.println(millis() - watch);
   if (packettimer != in.data[4]) {
     violationcount = 0;
   }
   else {
     violationcount++;
   }
   if (violationcount > 30) {
     //Really, something to terminate the execution should go here, but for testing
     shutdown();
   }
   if ((millis() - watch) < 20 && violationcount < 3) {
     //Serial.println(in.data);
     //Serial.println("joysticks:");
     //Serial.println(getLeftJoy(in).x);
     //Serial.println(getLeftJoy(in).y);
     //Serial.println(getRightJoy(in).x);
     vector2d leftjoy = squareToCircle(getLeftJoy(in));
     scalar_t rightjoy = getRightJoy(in).x;
     test = simpleMecanum(joyclamp(leftjoy), rightjoy, &joyScaleAbs);
     //Serial.print(test.wheel[0]); Serial.print(" ,");
     //Serial.print(test.wheel[1]); Serial.print(" ,");
     //Serial.print(test.wheel[2]); Serial.print(" ,");
     //Serial.print(test.wheel[3]); Serial.println(" ");
   }
   else {
     /*
       testTimer++;
       if (testTimer < 10) {
           test = simpleMecanum(vec(0.3,0.0),0.0,&joyScaleAbs);
           //run the motors for a second after the Arduino turns on
       }
       else {
           test = simpleMecanum(vec(0.0,0.0),0
       */
       test = simpleMecanum(vec(0.0,0.0),0.0,&joyScaleAbs);
   }
   packettimer = in.data[4];
   
   //turns out that 0 and 3 are inverted
   test.wheel[3] = -1.0 * test.wheel[3];
   test.wheel[0] = -1.0 * test.wheel[0];
   
   servotrain totry = drivetrainToServo(test);
   
   simpleServoDrive(totry, motors);
   delay(100);
   /*
   unsigned long gyrotimer = millis();
   while (millis() - gyrotimer < 100) { // 100 ms gyro sampling period
       rot = easyGyroPos(rot);
       delay(5);
   }
   */
   
   /*
   drivetrain test =  simpleMecanum(vec(0.5,0.0), 0.0, &joyScaleAbs); */
   //simpleServoDrive(drivetrainToServo(test), motors);
}
