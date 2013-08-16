#include <math.h>
typedef double scalar_t;
#if !defined(VECTORMATH)
#define VECTORMATH
#define PI 3.14159265357
/* KISS vector library. Currently only supports 2d vectors.
Compatible with C.

Contributors:
Alex Grabanski
*/
typedef double scalar_t;
typedef struct {scalar_t x; scalar_t y;} vector2d;
vector2d vec(scalar_t x, scalar_t y) {
    vector2d result;
    result.x = x;
    result.y = y;
    return result;
}
vector2d add(vector2d a, vector2d b) {
    return vec(a.x + b.x, a.y + b.y);
}
vector2d scale(vector2d a, scalar_t b) {
    return vec(a.x * b, a.y * b);
}
scalar_t dot(vector2d a, vector2d b) {
    return a.x * b.x + a.y * b.y;
}
scalar_t len(vector2d a) {
    return sqrt(a.x * a.x + a.y * a.y);
}
vector2d unit(vector2d a) {
    return scale(a, 1 / len(a));
}
vector2d flip(vector2d a) {
    return scale(a, -1.0);
}
vector2d sub(vector2d a, vector2d b) {
    return add(a, flip(b));
}
scalar_t normalizeAngle(scalar_t theta) {
    while (theta < 0.0) {
        theta += PI * 2;
    }
    while (theta > (PI * 2)) {
        theta -= PI * 2;
    }
    return theta;
}
    
scalar_t angle(vector2d a) {
    scalar_t result = atan2(a.y,a.x);
   return normalizeAngle(result); //result is in radians
}
vector2d polarvec(scalar_t angle, scalar_t r) {
    return vec(r * cos(angle), r * sin(angle));
}

scalar_t sec(scalar_t x) {
    return 1.0 / cos(x);
}

//This function takes an angle and returns the distance to the edge of
//a unit square from the origin [in that direction]
scalar_t distToSquare(scalar_t theta) {
    theta = normalizeAngle(theta);
    while (1 == 1) {
        //if it's on the right side of the square
        if (theta > (1.75 * PI) || theta < (.25 * PI)) {
            //give us the distance from the center to it
            return sec(theta);
        }
        //else, rotate the square and input by 90 degrees, try again
        theta = normalizeAngle(theta - (0.5 * PI));
    }
    return -1.0;
}

//This function turns square things into circular ones.
vector2d squareToCircle(vector2d in) {
    scalar_t theta = angle(in);
    scalar_t max_radius = distToSquare(theta);
    vector2d result = scale(in, 1.0 / max_radius);
    return result;
}

//this turns circular things into diamond-y things
//Note: greatest distance from center is sqrt(2)
vector2d circleToDiamond(vector2d in) {
    scalar_t theta = angle(in);
    //a diamond is just a square rotated 45 degrees
    scalar_t diamond_radius = distToSquare(theta + (0.25 * PI));
    //scale our circle up to the diamond
    vector2d result = scale(in, diamond_radius);
    return result;
}

//creates a "dead zone" for joysticks
vector2d joyclamp(vector2d in) {
    if (len(in) < 0.05) {
       return vec(0.0,0.0);
    }
    return in;
}

    
#endif
