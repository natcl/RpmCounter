#include <OSCMessage.h>
#include <OSCBoards.h>

#ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );
#else
#include <SLIPEncodedSerial.h>
 SLIPEncodedSerial SLIPSerial(Serial1);
#endif

#include <RpmCounter.h>

RpmCounter rpmCounter1;

void setup()
{
	rpmCounter1.setCountCallback(onCounter1);
	rpmCounter1.setRpmCallback(onRpm1);
	rpmCounter1.begin(3, 1, 400, 4, 10);
}

void loop()
{
	rpmCounter1.update();
}

void onRpm1(float val)
{
  OSCMessage msg("/rpm/1");
  msg.add( (float)val );
  
  SLIPSerial.beginPacket();  
  msg.send(SLIPSerial); // send the bytes to the SLIP stream
  SLIPSerial.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message

  delay(20);
}


void onCounter1()
{
	rpmCounter1.count();
}
