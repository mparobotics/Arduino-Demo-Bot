#include "vectormath.h"
#include "mecanumcommon.h"
/*Mecanum Inference

Beginners: In all likelihood, you actually want "mecanumdrive.h".

Contains functions that allow inferences about a mecanum drive.
Instead of joystick -> wheel voltage multipliers, this header
does wheel voltage multipliers -> actual speed, turn rate, heading.
This could potentially be used for motion planning, simulation,
diagnostics [warn when predicted acceleration gets out of sync with
the accelerometer], etc. etc. Also theoretically possible to create
an application where drivers can train to use mecanum drives
and programmers can test different methods of controlling mecanum
drives without access to an actual mecanum drive. 

Contributors:
Alex Grabanski */

#if !defined(MECANUMINFERENCE)
#define MECANUMINFERENCE

typedef scalar_t rpm;
typedef scalar_t voltage;
typedef scalar_t meters;

/* struct for inferences about instantaneous acceleration
without drag/friction factored in. */
typedef struct {
    vector2d strafeAccel;
    scalar_t turnAccel;
} mecanumInstant;

const rpm motorFree = 15000.0; //Motor free speed
const voltage maxVoltage = 12.00; //Motor max voltage
const meters wheelCircum = .0638371627; //Wheel circumference
const scalar_t sToMin = 1.0 / 60.0; //seconds to minutes

const scalar_t theoretAccelToActual = (sqrt(2.0) / 2.0) * 0.1; 
//accounts for torque, mass, etc
//FIXME: find a way to approximate this constant from mass, torque, etc
//Currently simply requires data collection and testing

const meters chlength = 0.5; //chassis length from wheel to wheel
const meters chheight = 0.5; //chassis height from wheel to wheel
//both are to the centers of the wheels

const scalar_t theoretTurnToActual = theoretAccelToActual * ((chlength + chheight) / 2.0);
//the above *should* approximately be right, but it may not be
//FIXME: work this out

//turns a voltage multiplier to actual voltage
voltage voltSclToReal(scalar_t mul) {
    return mul * maxVoltage;
}

/* this function outputs a theoretical velocity of any given
wheel provided the voltage supplied to it. [in meters per second]
This needs to be divided by some number to account for the
amount of force it takes to move the robot */
scalar_t voltsToRawAccel(voltage V) {
    return (motorFree / maxVoltage) * V * sToMin * wheelCircum;
}

/* This function infers strafing and turning acceleration
for the entire robot, independent of drag or friction,
which are proportional to velocity anyhow */
mecanumInstant inferAccel(drivetrain drivestate) {
    mecanumInstant result;
    scalar_t w0a = voltsToRawAccel(voltSclToReal(drivestate.wheel[0]));
    scalar_t w1a = voltsToRawAccel(voltSclToReal(drivestate.wheel[1]));
    scalar_t w2a = voltsToRawAccel(voltSclToReal(drivestate.wheel[2]));
    scalar_t w3a = voltsToRawAccel(voltSclToReal(drivestate.wheel[3]));
    result.strafeAccel.x = theoretAccelToActual * (w0a - w1a + w2a - w3a);
    result.strafeAccel.y = theoretAccelToActual * (w0a + w1a + w2a + w3a);
    result.turnAccel = theoretTurnToActual * (w1a - w0a + w2a - w3a);
    return result;
}

/* This function does accumulation with resistance/friction proportional
to velocity. initAccum is the current velocity, incr is the acceleration
force, Cf is the coefficient of friction [higher = more friction], and
t is time, with 0 being the time of initAccum. Returns the velocity
at time t*/
scalar_t accumFunc(scalar_t initAccum, scalar_t incr, scalar_t Cf, scalar_t t) {
    scalar_t k1 = initAccum - (incr / Cf);
    return (incr / Cf) + k1 * exp(-1 * t * Cf);
}

const scalar_t strafeCf = 2; /*coefficient of friction for strafing.
Can only be determined through testing and some smarts */

const scalar_t turnCf = strafeCf; /*At least, it theoretically should be
FIXME: But is it? */

typedef struct {
    vector2d strafeVel; //Components should be in meters per second. METRIC, USE IT
    scalar_t turnVel; //Components in radians per second. RADIANS, USE THEM!!
} mecanumVel;

/* Okay, now for the interesting stuff. This function takes the current velocity
and a force of instant acceleration to be applied over time t [seconds], and returns
the new velocity at the end of the time interval */
mecanumVel integrateAccel(mecanumVel init, mecanumInstant accelForce, scalar_t t) {
    mecanumVel result;
    result.strafeVel.x = accumFunc(init.strafeVel.x, accelForce.strafeAccel.x, strafeCf, t);
    result.strafeVel.y = accumFunc(init.strafeVel.y, accelForce.strafeAccel.y, strafeCf, t);
    result.turnVel = accumFunc(init.turnVel, accelForce.turnAccel, turnCf, t);
    return result;
}
/* This function is like the above, except it takes a drivetrain instead of
a mecanumInstant */
mecanumVel inferVel(mecanumVel init, drivetrain accelForce, scalar_t t) {
    return integrateAccel(init, inferAccel(accelForce), t);
}

typedef struct {
    mecanumVel velocity;
    vector2d position;
    scalar_t angle;
} mecanumPosAndVel;

/* This function infers both position and velocity when given an initial position
and velocity and a drivetrain and a time interval, and it returns a position and
a velocity.*/
mecanumPosAndVel inferPos(mecanumPosAndVel init, drivetrain drive, scalar_t t) {
    mecanumVel currentvel = init.velocity;
    mecanumVel nextvel = inferVel(currentvel, drive, t);
    mecanumPosAndVel result = init;
    
    /*Below, we use a trapezoidal approximation to the integral. This is an OK idea
    for accuracy's sake, but in theory, the error would be reduced if a higher-order
    method were used instead [such as simpson's rule]. FIXME, I guess. However,
    for all practical purposes, this should be fine, provided that the time intervals
    between summations are not too long. Before you ascribe position data variations
    to anything else in the math, write it off as a quirk of the integral approx */
    
    result.position.x += 0.5 * (nextvel.strafeVel.x + currentvel.strafeVel.x) * t;
    result.position.y += 0.5 * (nextvel.strafeVel.y + currentvel.strafeVel.y) * t;
    result.angle += 0.5 * (currentvel.turnVel + nextvel.turnVel) * t;
    result.velocity = nextvel;
    return result;
}

/*TODO: create another version of the above that uses Simpson's Rule or similar,
purely to be used for isolating inaccuracies in approximation */

#endif
