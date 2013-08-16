#include "mecanumcommon.h"
#include "joyconvert.h"
#ifdef __cplusplus
extern "C" {
#endif
#if !defined(MECANUMDRIVE)
#define MECANUMDRIVE
/*Library for actually doing the math required to drive a mecanum
drive.

Contributors:
Alexander Grabanski
*/

//assumptions: strafeJoy is a circular joystick, turnJoy is linearly scaled
//joyScalefptr returns a scaling factor to keep motor values in range
//while turning and moving at the same time
drivetrain simpleMecanum(vector2d strafeJoy, scalar_t turnJoy, 
                        scalar_t (*joyScalefptr)(scalar_t, scalar_t)) {
    //a transformation from the joystick coordinate system
    //to the rotated diamond system of a mecanum drive
    vector2d strafe = circleToDiamond(strafeJoy);
    //get the magnitude of the desired strafing movement
    scalar_t strafeMagnitude = len(strafeJoy);

    //get a multiplier for movement [1 - moveMult = multiplier for turns]
    scalar_t moveMult = (*joyScalefptr)(strafeMagnitude, turnJoy);
    scalar_t turnMult = 1.0 - moveMult;

    vector2d scaledStrafe = scale(strafe, moveMult);
    //invert, because we want negative turnJoy to mean
    //a positive [increasing theta] turn
    scalar_t scaledTurn = -1.0 * turnMult * turnJoy;

    //now, project the strafe vector onto the corresponding
    //unit direction vectors for each wheel
    //and add the contributions due to turning, as well.
    //wheels are numbered clockwise from front-left , and forward values
    //means positive values

    vector2d upRight = scale(vec(1,1), sqrt(2.0) / 2.0);
    vector2d upLeft = scale(vec(-1,1), sqrt(2.0) / 2.0);

    drivetrain result;
    result.wheel[0] = dot(scaledStrafe, upLeft) - scaledTurn;
    result.wheel[1] = dot(scaledStrafe, upRight) + scaledTurn;
    result.wheel[2] = dot(scaledStrafe, upLeft) + scaledTurn;
    result.wheel[3] = dot(scaledStrafe, upRight) - scaledTurn;
    return result;
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
#ifdef __cplusplus
}
#endif
