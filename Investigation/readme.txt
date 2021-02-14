Hardware/Protocol:
####################
RS 485 Modbus 57,6 kB - 8E1 LSB
IOLevel 0-3,2 V


Protocol:
##########
https://modbus.org/docs/Modbus_Application_Protocol_V1_1b.pdf

Request (Master):
92	SlaveID
17 	Function code: Read/Write Multiple registers
9C B9 	Read Starting Address 
00 05   Quantity to Read
9C 41   Write Starting Address
00 03   Quantity to Write
06  	Write Byte Count 
00 02 00 00 01 00
15 D9	CRC ModBUS

Response (Slave)
92	SlaveID
17	Function code
05	Byte Count
XX XX XX XX XX
YY YY   CRC ModBus


Antwortzeitverhalten Master<> Slave:
min 3974 ms
max 5916 ms


Kommunikation (siehe ProtocolAnalyse.txt):
nach dem BUS-Scan wird folgendes Muster immer wieder durchlaufen:

Master: Statusabfrage
	02 17 9C B9 00 08 9C 41 00 02 04 3E 03 00 00 EB CC 
Slave:  Status des Erweiterungsboards
	02 17 10 3E 00 03 01 00 00 00 00 00 00 00 00 00 00 00 00 74 1B 
Master (Broadcast): Status des Antriebs
	00 10 9D 31 00 09 12 16 00 C8 C8 20 60 00 00 00 00 00 00 00 14 00 01 00 00 BE E2


Besondere Nachrichten:
1. Beim Lichtschalten:
Master: 02 17 9C B9 00 02 9C 41 00 02 04 0F 04 17 00 7B 21	<-  0F zähler, 04 cmd?  data: 1700 (licht an) oder 1701 (licht aus)
Slave:  02 17 04 0F00 04FD 0A 72				<- 0F00 zähler, 04FD  04 cmd, FD result?  Konstante Nachricht?

2. Busscan
Master: 02 17 9C B9 00 05 9C 41 00 03 06 00 02 00 00 01 02 f8 35
Slave:  02 17 0a 00 00 02 05 04 30 10 ff a8 45 0e df
