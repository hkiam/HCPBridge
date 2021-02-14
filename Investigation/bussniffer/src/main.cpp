#include <Arduino.h>
#include <SoftwareSerial.h>


SoftwareSerial RS485(D6,D7);
#define RS485IF RS485
//#define RS485IF Serial


void setup()
{
  // Hörmann HCP2 based on modbus rtu @57.6kB 8E1  
  RS485IF.begin(57600,SWSERIAL_8E1);
  Serial.begin(115200,SERIAL_8N1);
}

#define T3_5 1750

#define BUFFERSIZE 255
int rxlen = 0;
int txlen = 0;
byte rxbbuffer[BUFFERSIZE];
byte txbbuffer[BUFFERSIZE];

unsigned long recvTime =0;
unsigned long recvStartTime =0;
unsigned long recvGapTime = 0;

void sendUShort(ushort data){
  Serial.write(data>>8);
  Serial.write(data&0xFF);
}

int loopcount = 0;
void loop()
{
  if(loopcount==0)
  {    
    rxlen=0;
    rxbbuffer[rxlen++] = 0x8;
    rxbbuffer[rxlen++] = 0xDE;
    rxbbuffer[rxlen++] = 0xAD;
    rxbbuffer[rxlen++] = 0xBE;
    rxbbuffer[rxlen++] = 0xEF;
    rxbbuffer[rxlen++] = 0xDE;
    rxbbuffer[rxlen++] = 0xAD;
    rxbbuffer[rxlen++] = 0xBE;
    rxbbuffer[rxlen++] = 0xEF;    
    
    Serial.write(rxbbuffer,rxlen);  

    rxlen=0;
    loopcount=1;
  }

  if(RS485IF.available() >0)
  {
      if(rxlen==0){
        recvStartTime = micros();
        recvGapTime = recvStartTime-recvTime;
      }
      rxlen+= RS485IF.readBytes((char*)(rxbbuffer+rxlen), _min(BUFFERSIZE-rxlen,RS485IF.available()));
      recvTime = micros(); 
      
  }

  if(rxlen>0 && (micros()-recvTime > T3_5 || rxlen > 0x80)) 
  {    
    Serial.write(rxlen);                            // Len
    sendUShort((short)recvGapTime);                 // Gap
    sendUShort((short)recvTime-recvStartTime);    // Packagelen
    Serial.write(rxbbuffer,rxlen);
    rxlen=0;
  }

  if(Serial.available() >0)
  {
      rxlen= Serial.readBytes((char*)(rxbbuffer), _min(BUFFERSIZE,Serial.available()));
      RS485IF.write(rxbbuffer,rxlen);
  }
}
