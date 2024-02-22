#include <WiFiUdp.h>

#include "Config.h"
extern int timeZone;

// wifi连接UDP设置参数
void sendNTPpacket(IPAddress &address);

time_t getNtpTime()         // 通过NTP获取时间
{
    IPAddress ntpServerIP;  // NTP server's ip address
    while (Udp.parsePacket() > 0)
        ;                   // discard any previously received packets
    // Serial.println("Transmit NTP Request");
    //  get a random server from the pool
    WiFi.hostByName(ntpServerName, ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            Serial.println("Receive NTP Response");
            Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900  = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    Serial.println("No NTP Response :-(");
    return 0;  // 无法获取时间时返回0
}

// 向NTP服务器发送请求
void sendNTPpacket(IPAddress &address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0]  = 0b11100011;  // LI, Version, Mode
    packetBuffer[1]  = 0;           // Stratum, or type of clock
    packetBuffer[2]  = 6;           // Polling Interval
    packetBuffer[3]  = 0xEC;        // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123);  // NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

void initNTP()
{
    // Login suceeded so set UDP local port
    Udp.begin(LOCALPORT);
    // Set the time provider to NTP
    setSyncProvider(getNtpTime);  // 同步时间
}

void time_update()
{
    setSyncProvider(getNtpTime);
}
