### Arduino EEPROM Read/Write

This is an implementation for reading/writing IP related information (IP Address, Subnet, Gateway and DNS) from/to Arduino EEPROM.
Tested on Arduino Ethernet board which has 1k of EEPROM but can be adapted to other Arduino boards.

## Available serial API:
- WRITE:IP:192.168.1.50      
- WRITE:SUBNET:255.255.255.0 
- WRITE:GATEWAY:192.168.1.254
- WRITE:DNS:192.168.1.254    
- READ:IP
- READ:SUBNET
- READ:GATEWAY
- READ:DNS
- RESTART:NET

## Assigned EEPROM Address:
- WRITE:IP:192.168.1.50          IP      : 0-3
- WRITE:SUBNET:255.255.255.0     SUBNET  : 4-7
- WRITE:GATEWAY:192.168.1.254    GATEWAY : 8-11
- DNS     : 12-15

## Example:
```
Serial port COM6 opened
READ:IP
IP : 192.168.1.100
WRITE:IP:192.168.1.50
WRITE OK
RESTART:NET
RESTART OK
READ:IP
IP : 192.168.1.50
```