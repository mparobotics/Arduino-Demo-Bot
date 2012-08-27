#include "vectormath.h"
#if !defined(MECANUMCOMMON)
#define MECANUMCOMMON
/* Common Mecanum code 
Contributors:
Alexander Grabanski */
typedef struct {
    scalar_t wheel[4];
}
drivetrain;
/* Some background info:
-All angles are in radians, from 0
facing right going up counter-clockwise.
-All wheel voltage multipliers being 1
implies that the robot moves at full forward
speed
-The wheels are numbered from the top left
going clockwise up from 0 to 3. 
*/
#define TL 0
#define TR 1
#define BR 2
#define BL 3
#endif
