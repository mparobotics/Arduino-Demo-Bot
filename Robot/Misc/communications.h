/* Communications code [client]-- Arduino 'bot 

Contributors:
Alex Grabanski */
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <vectormath.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xD5, 0x13 };
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
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write("acknowledged");
    Udp.endPacket();
    return result;
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
