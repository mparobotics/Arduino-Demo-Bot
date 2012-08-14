#include "vectormath.h"
#if !defined(JOYCONVERT)
#define JOYCONVERT
/* Joystick conversion library for holonomic drive

Implements a smarter approach for control of a holonomic
drive than simply scaling back inputs to fall within a required
range. The approach guarantees that full-forward, full-turn
will be different from half-forward, half-turn, enabling huge
gains in expressivity of control. Multiple functions can accomplish this
-- this library provides only two.
USE THIS!!! It will make control that much easier.
Note: Why so many comments? You must READ them and UNDERSTAND them. 
Graph the power curves for better intuitive understanding. 

Contributors:
Alex Grabanski
*/

/* This function returns a multiplier for strafing when given
two scalars, one for strafing, and one for turning, expressed
in the range -1.0 to 1.0. [max reverse to max forward.]
should be multiplied by the output voltage or joystick input
-- whichever is more convenient and readable for code. 
The multiplier for turning can be obtained by
1 - joyScaleAbs(_,_). The power curve of this function
for forward movement is given by y=((abs(x)+1)/2) * x 
where y is output scale and x is forward input scale*/
scalar_t joyScaleAbs(scalar_t strafeMag, scalar_t turnMag) {
    return (abs(strafeMag) - abs(turnMag) + 1)/2;
}

/* Same thing as joyScaleAbs, but squares each component instead
of taking the absolute value. Results in a consistent power curve
for individual components in a quadratic shape. You probably
want to use this for driving, because it also allows precision
movement in the low range. Power curve is given by
y = (x * x + 1)/2  *  x*/
scalar_t joyScaleSqr(scalar_t strafeMag, scalar_t turnMag) {
    return (strafeMag * strafeMag - turnMag * turnMag + 1)/2;
}

/* Same thing, but takes the square root of the square root of
the absolute value of each component. Why? It gives a roughly
linear power curve. Power curve given by
y = (sqrt(sqrt(abs(x))) + 1)/2  * x */
scalar_t joyScaleSqrt(scalar_t strafeMag, scalar_t turnMag) {
    return (sqrt(sqrt(abs(strafeMag))) - sqrt(sqrt(abs(turnMag))) + 1)/2;
}

/* This function is unrelated to the others -- it converts a
square joystick input with range [-1, 1], [-1, 1]
to a virtual circular joystick input with maximum radius of 1. */
vector2d joyScaleSquareCircle(vector2d square) {
    scalar_t squareradiusinv = cos(angle(square) - floor( (angle(square) + (.25 * PI)) / (.5 * PI)) * (.5 * PI));
    //if you were to see a polar graph of 1 / the above mostrosity, you'd see a square :P
    return scale(square, squareradiusinv);
}
#endif
