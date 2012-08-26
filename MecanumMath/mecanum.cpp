#include <iostream>
#include "joyconvert.h"
#include "vectormath.h"
#include "mecanumdrive.h"

int main() {
    scalar_t angle = 0.0;
    while (angle < (2.0 * 3.1415926535)) {
        drivetrain test = simpleMecanum(polarvec(angle, 1.0), 1.0, &joyScaleSqr);
        scalar_t max = 0.0;
        int i = 0;
        while (i < 4) {
            if (fabs(test.wheel[i]) > max) {
                max = fabs(test.wheel[i]);
            }
            i++;
        }
        std::cout << "angle: " << angle << " mul: " << 1.0 / max << "\n";
        angle += 3.1415926435 / 32.0;
    }
    /*
    std::cout << "wheel 0: " << test.wheel[0] << "\n";
    std::cout << "wheel 1: " << test.wheel[1] << "\n";
    std::cout << "wheel 2: " << test.wheel[2] << "\n";
    std::cout << "wheel 3: " << test.wheel[3] << "\n";
    */
    return 0;
}
