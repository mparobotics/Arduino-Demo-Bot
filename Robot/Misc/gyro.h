/* Gyro.h -- Reads gyroscope data [Partially Arduino-specific]

Contributors:
Alexander Grabanski
*/
#define GYROPIN 1
#define GYROMAX 5
#define GYROZERO 2.501
#define GYROSENS .007   //gyro sensitivity in V/deg/sec
#define GYTHRESH 1 //Minimum deg/sec to keep track of -- helps with drift
#if !defined GYROH
#define GYROH

//this function returns the voltage of the gyro pin.
scalar_t readGyroInstantV() {
    return (analogRead(GYROPIN) * GYROMAX) / 1023.0;
}
//this function returns the instantaneous rate of change 
//(in radians/sec) given the gyro's voltage
scalar_t gyroVtoRate(scalar_t voltage) {
    return radians((voltage - GYROZERO) / GYROSENS);
}
scalar_t normalizeAngle(scalar_t angle) {
    if (angle > (2.0 * PI)) {
        return angle - (2.0 * PI);
    }
    if (angle < 0) {
        return angle + (2.0 * PI);
    }
    return angle;
}
/*When given a previous gyro rotation [in radians] and a amount of time between
calling this function [in seconds], this function returns the new rotation
This function is time-dependent. */
scalar_t gyroPos(scalar_t gyroPrev, scalar_t t) {
    return normalizeAngle(gyroPrev + gyroVtoRate(readGyroInstantV()) * t);
}

/*And this function is a different version of the above one that keeps
hidden state [!] so a time parameter is unnecessary. While it's still
a valid approximation, this function will give different results depending
on how often it is called. */
unsigned long gyroHiddenTime = 0;
scalar_t easyGyroPos(scalar_t gyroPrev) {
    unsigned long newgyroTime = millis();
    scalar_t result = gyroPos(gyroPrev, (newgyroTime - gyroHiddenTime) / 1000.0);
    gyroHiddenTime = newgyroTime;
    return result;
}
#endif
