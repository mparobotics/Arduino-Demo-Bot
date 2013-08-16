/* Drive.h -- Generic drivetrain code [Arduino-Specific]

Contributors:
Alexander Grabanski */
#include <mecanumdrive.h>
#include <Servo.h>
typedef struct {
    scalar_t wheel[4];
} servotrain;

scalar_t voltMulToServo(scalar_t voltMul) {
    return voltMul * 90 + 90;
}
servotrain drivetrainToServo(drivetrain train) {
    servotrain result;
    result.wheel[0] = voltMulToServo(train.wheel[0]);
    result.wheel[1] = voltMulToServo(train.wheel[1]);
    result.wheel[2] = voltMulToServo(train.wheel[2]);
    result.wheel[3] = voltMulToServo(train.wheel[3]);
    return result;
}
void simpleServoDrive(servotrain in, Servo out[4]) {
    out[0].write(in.wheel[0]);
    out[1].write(in.wheel[1]);
    out[2].write(in.wheel[2]);
    out[3].write(in.wheel[3]);
    return;
}
