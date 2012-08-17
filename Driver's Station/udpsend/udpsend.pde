 // Processing UDP example to send and receive string data from Arduino 
 // press any key to send the "Hello Arduino" message
 
 
 import hypermedia.net.*;
 
import procontroll.*;
import net.java.games.input.*;

ControllIO controllIO;
ControllDevice joypad;
ControllSlider stickx;
ControllSlider sticky;
ControllButton trigger;
 
 UDP udp;  // define the UDP object
 
 
 void setup() {
 udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
 //udp.log( true );         // <-- printout the connection activity
 udp.listen( true );           // and wait for incoming message  
 
  controllIO = ControllIO.getInstance(this);
  joypad = controllIO.getDevice("Logitech Attack 3");
  
  trigger = joypad.getButton(0);
  stickx = joypad.getSlider(0);
  sticky = joypad.getSlider(1);
 }
 
 void draw()
 {
   String ip       = "192.168.1.178"; // the remote IP address
   int port        = 8888;        // the destination port
   float joystickx = stickx.getValue();
   println(str(joystickx));
   udp.send(str(joystickx), ip, port );   // the message to send
 }
 
 void keyPressed() {
 String ip       = "192.168.1.178"; // the remote IP address
 int port        = 8888;        // the destination port
 
 udp.send("Hello World", ip, port );   // the message to send
 
 }
 
 //void receive( byte[] data ) {          // <-- default handler
 //void receive( byte[] data, String ip, int port ) {   // <-- extended handler
 /*
 for(int i=0; i < data.length; i++) 
 print(char(data[i]));  
 println();   
 } */
