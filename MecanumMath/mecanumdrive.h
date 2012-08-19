#include "mecanumcommon.h"
#include "joyconvert.h"
#if !defined(MECANUMDRIVE)
#define MECANUMDRIVE
/*Library for actually doing the math required to drive a mecanum
drive.

Contributors:
Alexander Grabanski
*/

/* I understand HOW this function works, but I don't understand the why. 
It just works. Without it, the motors cannot give maximum speed. I derived
it by doing automated testing and guessing at the nature of the problem
and seeing that it worked. However, there should be nothing wrong about
this function -- I am absolutely certain that it works -- I have verified
it graphically. However, I am uncertain as to WHY it works. */

scalar_t magicFunc(scalar_t angle) {
    return 1.0 / sin(fmod(angle, 3.1415926535 / 2.0 ) + (3.1415926535 / 4.0));
}

/* moveAngle in radians, moveMult is [-1, 1] {1 being forward}, 
turnMult is [-1, 1] {clockwise rate of turning with 1 being max}. */
drivetrain mecanumBasicPolar(scalar_t moveAngle, scalar_t moveMult, scalar_t turnMult) {
     drivetrain result;
     scalar_t magic = magicFunc(moveAngle);
     result.wheel[0] = moveMult * cos(moveAngle - (PI / 4.0)) * magic - turnMult;
     result.wheel[1] = moveMult * sin(moveAngle - (PI / 4.0)) * magic - turnMult;
     result.wheel[2] = moveMult * cos(moveAngle - (PI / 4.0)) * magic + turnMult;
     result.wheel[3] = moveMult * sin(moveAngle - (PI / 4.0)) * magic + turnMult;
     return result;
}
/* Takes a circular joystick input for strafing and an input for turning,
then takes a pointer to a joystick conversion function (e.g: &joyScaleAbs)
Automagically returns a drivetrain struct from given data
This is the "smart, but simple" approach to mecanum control, and it is
currently the only one in this header that provides and easy way
to give movement relative to a driver.
*/
drivetrain simpleMecanum(vector2d strafeJoy, scalar_t turnJoy, 
                        scalar_t (*joyScalefptr)(scalar_t, scalar_t)) {
    scalar_t moveMult = (*joyScalefptr)(len(strafeJoy), turnJoy);
    return mecanumBasicPolar(angle(strafeJoy), len(strafeJoy) * moveMult, turnJoy * (1 - moveMult));
}


/* This is the stupidest approach to Mecanum drive code that works.
Movement is relative to the robot. Here's a visual [< = left,
> = right, ^ = up, v = down. Left side is the joystick, right side
is the motors 

^ ^     ^ ^   --full forward
        ^ ^

v v     v v   --full reverse
        v v

v ^     v ^   --counter-clockwise turning
        v ^

< <     v ^   --strafe left
        ^ v

> >     ^ v  --strafe right
        v ^ 

Also, diagonal directions should theoretically work as intended.
Cons:
It's really dumb.
Pros:
It's really dumb. */
scalar_t clip(scalar_t val, scalar_t low, scalar_t high) {
    if (val > high) {
        return high;
    }
    if (val < low) {
        return low;
    }
    return val;
}
drivetrain stupidMecanum(vector2d leftJoy, vector2d rightJoy) {
    drivetrain result;
    result.wheel[0] = len(leftJoy) * clip(leftJoy.x + leftJoy.y, -1.0, 1.0);
    result.wheel[1] = len(rightJoy) * clip(rightJoy.y - rightJoy.x, -1.0, 1.0);
    result.wheel[2] = len(rightJoy) * clip(rightJoy.y + rightJoy.x, -1.0, 1.0);
    result.wheel[3] = len(leftJoy) * clip(leftJoy.y - leftJoy.x, -1.0, 1.0);
    return result;
}

#endif
