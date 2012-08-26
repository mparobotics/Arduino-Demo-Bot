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
  joypad2 = controlIO.getDevice("Logitech Attack 3");
  
  trigger = joypad.getButton(0);
  stickx1 = joypad.getSlider(0);
  sticky1 = joypad.getSlider(1);
  stickx2 = joypad2.getSlider(0);
  sticky2 = joypad2.getSlider(1);
 }
 
byte[] toBytes(float f) {
    int i = floatToIntBits(f);
    byte[] result = new byte[4];

    result[0] = (byte) (i >> 24);
    result[1] = (byte) (i >> 16);
    result[2] = (byte) (i >> 8);
    result[3] = (byte) (i);

    return result;
}
byte[] concat(byte[] A, byte[] B) {
    byte[] C = new[A.length + B.length];
    System.arraycopy(A, 0, C, 0, A.length);
    System.arraycopy(B, 0, C, A.length, B.length);
    return C;
}
byte[] concat(byte[] A, byte[] B, byte[] C, byte[] D) {
    concat(concat(A,B), concat(C,D));
}
byte[] concat(float A, float B, float C, float D) {
    concat(toBytes(A), toBytes(B), toBytes(C), toBytes(D));
}

 void draw()
 {
   String ip       = "192.168.1.178"; // the remote IP address
   int port        = 8888;        // the destination port
   float joyx1 = stickx1.getValue();
   float joyy1 = sticky1.getValue();
   float joyx2 = stickx2.getValue();
   float joyy2 = sticky2.getValue();
   udp.send(concat(joyx1, joyy1, joyx2, joyy2), ip, port );   // the message to send
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
