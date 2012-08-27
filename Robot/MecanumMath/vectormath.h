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
scalar_t angle(vector2d a) {
    scalar_t result = atan2(a.y,a.x);
    if (result < 0.0) {
        result += PI * 2;
    }
    return result; //result is in radians
}
vector2d polarvec(scalar_t angle, scalar_t r) {
    return vec(r * cos(angle), r * sin(angle));
}
#endif
