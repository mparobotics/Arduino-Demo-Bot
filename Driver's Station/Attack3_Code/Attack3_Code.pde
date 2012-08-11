import procontroll.*;
import net.java.games.input.*;

ControllIO controllIO;
ControllDevice joypad;

ControllSlider stickx;
ControllSlider sticky;
ControllSlider throttle;

ControllButton trigger;
ControllButton button2;
ControllButton button3;
ControllButton button4;
ControllButton button5;
ControllButton button6;
ControllButton button7;
ControllButton button8;
ControllButton button9;
ControllButton button10;
ControllButton button11;
ControllButton button12;



void setup() {
  
  size (100,100);
  
  controllIO = ControllIO.getInstance(this);
  joypad = controllIO.getDevice("Logitech Attack 3");
  
  trigger = joypad.getButton(0);
  button2 = joypad.getButton(1);
  button3 = joypad.getButton(2);
  button4 = joypad.getButton(3);
  button5 = joypad.getButton(4);
  button6 = joypad.getButton(5);
  button7 = joypad.getButton(6);
  button8 = joypad.getButton(7);
  button9 = joypad.getButton(8);
  button10 = joypad.getButton(9);
  button11 = joypad.getButton(10);
  button12 = joypad.getButton(11);

  sticky = joypad.getSlider(0);
  stickx = joypad.getSlider(1);
  throttle = joypad.getSlider(2);
  
}

void draw() {
 
}
