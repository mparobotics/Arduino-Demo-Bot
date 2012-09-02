/* Communications code [client]-- Arduino 'bot 

Contributors:
Alex Grabanski */
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <vectormath.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xD5, 0x13 };
IPAddress ip(192, 168, 1, 178);

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
    packet result;
    Udp.read(&result.data,UDP_TX_PACKET_MAX_SIZE);
    return result;
}
scalar_t toAxis(char b) {
    return (b / 127.0);
}
vector2d getLeftJoy(packet p) {
    return vec(toAxis(p[0]), toAxis(p[1]));
}
vector2d getRightJoy(packet p) {
    return vec(toAxis(p[2]), toAxis(p[3]));
}
