/* Communications code [client]-- Arduino 'bot 

Contributors:
Alex Grabanski */

#ifndef COMMUNICATIONS_DEFINED
#define COMMUNICATIONS_DEFINED

#include <Ethernet.h>
#include <EthernetUdp.h>

//byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xD5, 0x13 };
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x45, 0x55 };
IPAddress ip(10, 39, 26, 123);

unsigned int localPort = 8888;

typedef union {
    char data[UDP_TX_PACKET_MAX_SIZE]; 
    char joystickaxes[4]; //counting on the above being larger
} packet;

EthernetUDP Udp;

void netSetup() {
    Ethernet.begin(mac, ip);
    Udp.begin(localPort);
}
packet readPacket() {
    int packetSize = Udp.parsePacket();
    packet result;
    Udp.read(result.data,UDP_TX_PACKET_MAX_SIZE);
    // send reply
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write("acknowledged");
    //Udp.endPacket();
    return result;
}
void sendPacket(packet in) {
      Udp.parsePacket();
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write((uint8_t*) in.data, UDP_TX_PACKET_MAX_SIZE);
      Udp.endPacket();
      return;
}

scalar_t toAxis(char b) {
    return (b / 127.0);
}
vector2d getLeftJoy(packet p) {
    return vec(toAxis(p.joystickaxes[0]), toAxis(p.joystickaxes[1]));
}
vector2d getRightJoy(packet p) {
    return vec(toAxis(p.joystickaxes[2]), toAxis(p.joystickaxes[3]));
}
   
#endif 