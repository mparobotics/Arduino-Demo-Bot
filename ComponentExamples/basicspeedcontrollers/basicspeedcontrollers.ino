#include <Servo.h>

Servo victor;
Servo jag;

void setup() {
  victor.attach(5);
  jag.attach(6);
}

void loop() {
  victor.write(85); //5 degrees backwards from full stop
                    //the range is 0 [full backwards] -> 90 [neutral] ->
                    //180 [full forwards]
  jag.write(100); //tested 95, happens to be in a "stall" range
                  // more testing is necessary. 
}
