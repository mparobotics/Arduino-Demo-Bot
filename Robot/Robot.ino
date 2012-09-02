#include <Servo.h>

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
}
void loop() {
   /*
   packet in = readPacket();
   drivetrain test = simpleMecanum(getLeftJoy(in), getRightJoy(in).x, &joyScaleAbs);
   */
   drivetrain test =  simpleMecanum(vec(0.5,0.0), 0.0, &joyScaleAbs);
   simpleServoDrive(drivetrainToServo(test), motors);
}
