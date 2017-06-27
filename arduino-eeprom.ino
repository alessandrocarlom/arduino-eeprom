/*
   Info     : Serial interface for storing/retrieving
              IP address information to/from EEPROM
   Created  : 19 June 2017
   Author   : Eddy Yanto
   Board    : Arduino Ethernet

   Available serial commands      EEPROM Address
   ---------------------------    --------------
   WRITE:IP:192.168.1.50          IP      : 0-3
   WRITE:SUBNET:255.255.255.0     SUBNET  : 4-7
   WRITE:GATEWAY:192.168.1.254    GATEWAY : 8-11
   WRITE:DNS:192.168.1.254        DNS     : 12-15
   READ:IP
   READ:SUBNET
   READ:GATEWAY
   READ:DNS
   RESTART:NET
*/

#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0x20, 0xF8, 0x5E, 0xEF, 0xFE, 0xED };

char buff[32]; //buffer for sprintf
char byteSerial; //store incoming serial one byte at a time
String inputSerial, command, param; //command parsing variable

//EEPROM address
const int ipEEPROMAddress[]       = {0, 1, 2, 3};
const int subnetEEPROMAddress[]   = {4, 5, 6, 7};
const int gatewayEEPROMAddress[]  = {8, 9, 10, 11};
const int dnsEEPROMAddress[]      = {12, 13, 14, 15};

IPAddress ip(EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3));
IPAddress mdns(EEPROM.read(12), EEPROM.read(13), EEPROM.read(14), EEPROM.read(15));
IPAddress gateway(EEPROM.read(8), EEPROM.read(9), EEPROM.read(10), EEPROM.read(11));
IPAddress subnet(EEPROM.read(4), EEPROM.read(5), EEPROM.read(6), EEPROM.read(7));
EthernetServer tcpServer(7000);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip, mdns, gateway, subnet);
  tcpServer.begin();
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void loop() {
  __processSerial();
}

void __processSerial() {
  if (Serial.available() > 0) {
    byteSerial = Serial.read();
    inputSerial += byteSerial;

    if (byteSerial == '\r') {
      inputSerial.trim();

      //parse COMMAND: WRITE or READ — eg: WRITE:IP:192.168.1.50
      int i = inputSerial.indexOf(":"); //421
      command = inputSerial.substring(0, i);

      //trim COMMAND from input — eg:
      //WRITE:IP:192.168.1.50
      //IP:192.168.1.50
      inputSerial = inputSerial.substring(i + 1, inputSerial.length());

      //parse PARAM: IP or SUBNET or GATEWAY or DNS
      //IP:192.168.1.50
      //192.168.1.50
      i = inputSerial.indexOf(":");
      param = inputSerial.substring(0, i);

      if (command == "WRITE") {
        //trim PARAM from input
        inputSerial = inputSerial.substring(i + 1, inputSerial.length()); //eg: 192.168.1.50

        IPAddress tempIp;
        int valid = tempIp.fromString(inputSerial);

        if (valid) {
          if (param == "IP") {
            __writeEEPROM(ipEEPROMAddress, tempIp);
          } else if (param == "SUBNET") {
            __writeEEPROM(subnetEEPROMAddress, tempIp);
          } else if (param == "GATEWAY") {
            __writeEEPROM(gatewayEEPROMAddress, tempIp);
          } else if (param == "DNS") {
            __writeEEPROM(dnsEEPROMAddress, tempIp);
          } else {
            Serial.println("\nUNKNOWN COMMAND");
          }
        } else {
          Serial.println("\nINVALID ADDRESS");
        }
      } else if (command == "READ") {
        if (param == "IP") {
          sprintf(buff, "\nIP : %d.%d.%d.%d", EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3));
          Serial.println(buff);
        } else if (param == "SUBNET") {
          sprintf(buff, "\nSUBNET : %d.%d.%d.%d", EEPROM.read(4), EEPROM.read(5), EEPROM.read(6), EEPROM.read(7));
          Serial.println(buff);
        } else if (param == "GATEWAY") {
          sprintf(buff, "\nGATEWAY : %d.%d.%d.%d", EEPROM.read(8), EEPROM.read(9), EEPROM.read(10), EEPROM.read(11));
          Serial.println(buff);
        } else if (param == "DNS") {
          sprintf(buff, "\nDNS : %d.%d.%d.%d", EEPROM.read(12), EEPROM.read(13), EEPROM.read(14), EEPROM.read(15));
          Serial.println(buff);
        } else {
          Serial.println("\nUNKNOWN COMMAND");
        }
      } else if (command == "RESTART") {
        Serial.println("\nRESTART OK");
        delay(50);
        resetFunc();
      } else {
        Serial.println("\nUNKNOWN COMMAND");
      }

      inputSerial = "";
      command = "";
      param = "";
    }
  }
}

void __writeEEPROM(int add[4], IPAddress tempIp) {
  for (int i = 0; i <= 4; i++) {
    EEPROM.write(add[i], tempIp[i]);
  }
  Serial.println("\nWRITE OK");
}
