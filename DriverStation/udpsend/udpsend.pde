 // Processing UDP example to send and receive string data from Arduino 
 // press any key to send the "Hello Arduino" message
 
 
 import hypermedia.net.*;
 
import procontroll.*;
import net.java.games.input.*;

ControllIO controllIO;
ControllDevice joypad;
ControllDevice joypad2;
ControllSlider stickx1;
ControllSlider stickx2;
ControllSlider sticky1;
ControllSlider sticky2;
ControllButton trigger;
 
 UDP udp;  // define the UDP object
 
 
 void setup() {
   udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
   //udp.log( true );         // <-- printout the connection activity
   //udp.listen( true );           // and wait for incoming message  
 
   controllIO = ControllIO.getInstance(this);
   joypad = controllIO.getDevice("Logitech Attack 3");
   joypad2 = controllIO.getDevice("Logitech Attack 3");
  
   trigger = joypad.getButton(0);
   stickx1 = joypad.getSlider(0);
   sticky1 = joypad.getSlider(1);
   stickx2 = joypad2.getSlider(0);
   sticky2 = joypad2.getSlider(1);
 }
 
/*this function converts from a floating-point in the range of -1.0 to 1.0 to an
integer [byte] with the range -127 -> 127 */
byte[] convertControl(float A) {
    byte[] result = new byte[1];
    result[0] = (byte)((int)(A * 127.0));
    return result;
}

byte[] concatbytes(byte[] A, byte[] B) {
    byte[] C = new byte[A.length + B.length];
    System.arraycopy(A, 0, C, 0, A.length);
    System.arraycopy(B, 0, C, A.length, B.length);
    return C;
}
byte[] concatbytes(byte[] A, byte[] B, byte[] C, byte[] D) {
    return concatbytes(concatbytes(A,B), concatbytes(C,D));
}
byte[] concatbytes(float A, float B, float C, float D) {
    return concatbytes(convertControl(A), convertControl(B), 
    convertControl(C), convertControl(D));
}

 void draw()
 {
   String ip       = "192.168.1.178"; // the remote IP address
   int port        = 8888;        // the destination port
   float joyx1 = stickx1.getValue();
   float joyy1 = sticky1.getValue();
   float joyx2 = stickx2.getValue();
   float joyy2 = sticky2.getValue();
   udp.send(concatbytes(joyx1, joyy1, joyx2, joyy2), ip, port );   // the message to send
   delay(33); //Keep us operating at about 30 packets per second
              //FIXME: determine reasonable limits for this
 }
/* 
 void keyPressed() {
 String ip       = "192.168.1.178"; // the remote IP address
 int port        = 8888;        // the destination port
 
 udp.send("Hello World", ip, port );   // the message to send
   println(concatbytes(1.0, 0.5, 0.0, -0.5));
 
 } */
 
 //void receive( byte[] data ) {          // <-- default handler
 //void receive( byte[] data, String ip, int port ) {   // <-- extended handler
 /*
 for(int i=0; i < data.length; i++) 
 print(char(data[i]));  
 println();   
 } */
