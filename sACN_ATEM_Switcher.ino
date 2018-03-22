/*****************
 *  Translates sACN input to ATEM for Black Magic Switchers
 *  Currently Monitors DMX channel 1 for Aux State, 2 for Program Input Source,
 *  3 for Preview Input Source, and Channel 10 for Making cuts.
 *
 *  URL :   https://github.com/whoismikesmith/sACN_ATEM_Switcher
 *
 *  Based on work from skaarhoj.com under the CC-by-sa license
 *  Read more at http://www.skaarhoj.com/about/licenses/#0mCuP9t1w8roJmCh.99
 *
 *  E1.31 Receiver and pixel controller by Andrew Huxtable (andrew@hux.net.au)
*/
/*****************

 * TO MAKE THIS EXAMPLE WORK:
 * - You must have an Arduino with Ethernet Shield (or compatible such as "Arduino Ethernet", http://arduino.cc/en/Main/ArduinoBoardEthernet)
 * - You must have an Atem Switcher connected to the same network as the Arduino - and you should have it working with the desktop software
 * - You must make specific set ups in the below lines where the comment "// SETUP" is found!
 */

/*
	IMPORTANT: If you want to use this library in your own projects and/or products,
 	please play a fair game and heed the license rules! See our web page for a Q&A so
 	you can keep a clear conscience: http://skaarhoj.com/about/licenses/
 */


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <Streaming.h>
#include <EthernetUdp.h>

// Include ATEMbase library and make an instance:
// The port number is chosen randomly among high numbers.
#include <ATEMbase.h>
#include <ATEMext.h>
#include <SkaarhojBufferTools.h>

// enter desired universe and subnet (sACN first universe is 1)
#define DMX_SUBNET 0
#define DMX_UNIVERSE 1 //**Start** universe

// MAC address and IP address for this *particular* Arduino / Ethernet Shield!
// The MAC address is printed on a label on the shield or on the back of your device
// The IP address should be an available address you choose on your subnet where the switcher is also present
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0D, 0x6B, 0xB9
};      // <= SETUP!  MAC address of the Arduino
IPAddress ip(192, 168, 10, 99);        // <= SETUP!  IP address of the Arduino
IPAddress switcherIp(192, 168, 10, 240);     // <= SETUP!  IP address of the ATEM Switcher

IPAddress netmask = (255, 255, 255, 0); //<= for sACN;
IPAddress gateway = (192, 168, 10, 1);  //<= for sACN;

EthernetUDP Udp;

// By sacrificing some of the Ethernet receive buffer, we can allocate more to the LED array
// but this is **technically** slower because 2 packets must be processed for all 240 pixels.

/// DONT CHANGE unless you know the consequences...
#define ETHERNET_BUFFER 540
#define CHANNEL_COUNT 100 //because it divides by 3 nicely
//#define NUM_LEDS 240 // can not go higher than this - Runs out of SRAM
#define UNIVERSE_COUNT 1
#define CHANNELS_PER_UNIVERSE 512

ATEMext AtemSwitcher;

//************************************************** ******************************

unsigned char packetBuffer[ETHERNET_BUFFER];

uint8_t aux = 0;
uint8_t me = 0;
uint16_t input1 = 1;
uint16_t input2 = 2;
uint16_t input3 = 3;
uint16_t source1 = 1;
uint16_t source2 = 2;
uint16_t source3 = 3;
uint16_t source4 = 4;
uint16_t source5 = 5;
uint16_t source6 = 6;
uint16_t source7 = 7;
uint16_t source8 = 8;
uint16_t bars = 1000;

void atemLoop() {
  AtemSwitcher.setAuxSourceInput(aux, input1);

  AtemSwitcher.setAuxSourceInput(aux, input2);

  AtemSwitcher.setAuxSourceInput(aux, bars);

  }



void udpLoop(unsigned char* p){
  // all dmx channels referenced in buffer by p[***].
  // 0-125 is the sACN header. dmx channel 1 starts at p[126]

  //***** Aux Channel (channel 1 more than 50% = aux1=1, less than 50% = aux1=2)
   if (p[126] > 128){
            Serial.print("Aux Switch to 1 \n");
            Serial.println(p[126]);
            AtemSwitcher.setAuxSourceInput(aux, input1);
          }
    if (p[126] < 128) {
            Serial.print("Aux Switch to 2 \n");
            Serial.println(p[126]);
            AtemSwitcher.setAuxSourceInput(aux, input2);
          }

    //***** Program Inputs (dmx channel 2: 0-12.5% = source1, 12.6%-25% = source2, etc...)
 if (p[127] > 0 && p[127] < 32){
            Serial.print("PGM Switch to 1 \n");
            Serial.println(p[127]);
            AtemSwitcher.setPreviewInputVideoSource(me, source1);
            //AtemSwitcher.performCutME(me);
          }
     if (p[127] > 33 && p[127] < 64){
            Serial.print("PGM Switch to 2 \n");
            Serial.println(p[127]);
            AtemSwitcher.setPreviewInputVideoSource(me, source2);
            //AtemSwitcher.performCutME(me);
          }
    if (p[127] > 65 && p[127] < 96) {
            Serial.print("PGM Switch to 3 \n");
            Serial.println(p[127]);
            AtemSwitcher.setPreviewInputVideoSource(me, source3);
            //AtemSwitcher.performCutME(me);
          }
    if (p[127] > 97 && p[127] < 128) {
            Serial.print("PGM Switch to 4 \n");
            Serial.println(p[127]);
            AtemSwitcher.setPreviewInputVideoSource(me, source4);
            //AtemSwitcher.performCutME(me);
          }
      if (p[127] > 129 && p[127] < 160){
            Serial.print("PGM Switch to 5 \n");
            Serial.println(p[127]);
            AtemSwitcher.setPreviewInputVideoSource(me, source5);
            //AtemSwitcher.performCutME(me);
          }
     if (p[127] > 161 && p[127] < 192){
            Serial.print("PGM Switch to 6 \n");
            Serial.println(p[127]);
            AtemSwitcher.setPreviewInputVideoSource(me, source6);
            //AtemSwitcher.performCutME(me);
          }
    if (p[127] > 193 && p[127] < 224) {
            Serial.print("PGM Switch to 7 \n");
            Serial.println(p[127]);
            AtemSwitcher.setPreviewInputVideoSource(me, source7);
            //AtemSwitcher.performCutME(me);
          }
    if (p[127] > 225 && p[127] < 255) {
            Serial.print("PGM Switch to 8 \n");
            Serial.println(p[127]);
            AtemSwitcher.setPreviewInputVideoSource(me, source8);
            //AtemSwitcher.performCutME(me);
          }
      //***** Preview Inputs (dmx channel 3: 0-12.5% = source1, 12.6%-25% = source2, etc...)
      if (p[128] > 0 && p[128] < 32){
            Serial.print("PRV Switch to 1 \n");
            Serial.println(p[128]);
            AtemSwitcher.setPreviewInputVideoSource(me, source1);
            //AtemSwitcher.performCutME(me);
          }
     if (p[128] > 33 && p[128] < 64){
            Serial.print("PRV Switch to 2 \n");
            Serial.println(p[128]);
            AtemSwitcher.setPreviewInputVideoSource(me, source2);
            //AtemSwitcher.performCutME(me);
          }
    if (p[128] > 65 && p[128] < 96) {
            Serial.print("PRV Switch to 3 \n");
            Serial.println(p[128]);
            AtemSwitcher.setPreviewInputVideoSource(me, source3);
            //AtemSwitcher.performCutME(me);
          }
    if (p[128] > 97 && p[128] < 128) {
            Serial.print("PRV Switch to 4 \n");
            Serial.println(p[128]);
            AtemSwitcher.setPreviewInputVideoSource(me, source4);
            //AtemSwitcher.performCutME(me);
          }
      if (p[128] > 129 && p[128] < 160){
            Serial.print("PRV Switch to 5 \n");
            Serial.println(p[128]);
            AtemSwitcher.setPreviewInputVideoSource(me, source5);
            //AtemSwitcher.performCutME(me);
          }
     if (p[128] > 161 && p[128] < 192){
            Serial.print("PRV Switch to 6 \n");
            Serial.println(p[128]);
            AtemSwitcher.setPreviewInputVideoSource(me, source6);
            //AtemSwitcher.performCutME(me);
          }
    if (p[128] > 193 && p[128] < 224) {
            Serial.print("PRV Switch to 7 \n");
            Serial.println(p[128]);
            AtemSwitcher.setPreviewInputVideoSource(me, source7);
            //AtemSwitcher.performCutME(me);
          }
    if (p[128] > 225 && p[128] < 255) {
            Serial.print("PRV Switch to 8 \n");
            Serial.println(p[128]);
            AtemSwitcher.setPreviewInputVideoSource(me, source8);
            //AtemSwitcher.performCutME(me);
          }
   //***** Cuts (dmx channel 10: higher than 50% sends cut command)
          //if dmx channel 10 is more than 50% perform cut
   if (p[136] > 128){
            Serial.print("Aux Switch to 1 \n");
            Serial.println(p[136]);
            AtemSwitcher.performCutME(me);
          }

  }
//*** Run testLoopA to autoscroll through program input video sources
void testLoopA(){
  AtemSwitcher.setProgramInputVideoSource(me, source1);
  delay(50);
  AtemSwitcher.setProgramInputVideoSource(me, source2);
  delay(50);
  AtemSwitcher.setProgramInputVideoSource(me, source3);
  delay(50);
  AtemSwitcher.setProgramInputVideoSource(me, source4);
  delay(50);
  }

  //*** Run testLoopB to autoscroll through aux channels
  // this was useful to me to see the buttons on the face of the switcher changing state.
void testLoopB(){
   AtemSwitcher.setAuxSourceInput(aux, input1);
  delay(50);
   AtemSwitcher.setAuxSourceInput(aux, input2);
  delay(50);
   AtemSwitcher.setAuxSourceInput(aux, input3);
  delay(50);
   AtemSwitcher.setAuxSourceInput(aux, bars);
  delay(50);
  }

void setup() {

  randomSeed(analogRead(5));  // For random port selection

  // Start the Ethernet, Serial (debugging) and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(5568);
  Serial.begin(115200);
  Serial << F("\n- - - - - - - -\nSerial Started\n");

  // Initialize a connection to the switcher:
  AtemSwitcher.begin(switcherIp);
  AtemSwitcher.serialOutput(2);
  AtemSwitcher.connect();


}

void loop() {
  AtemSwitcher.runLoop();

  //Process packets
int packetSize = Udp.parsePacket(); //Read UDP packet count
if(packetSize){
Udp.read(packetBuffer,ETHERNET_BUFFER); //read UDP packet
int count = checkACNHeaders(packetBuffer, packetSize);
if (count) {
sacnDMXReceived(packetBuffer, count); //process data function
}
}
  //Run any test loops here

  //testLoopA();
  //testLoopB();

}

void sacnDMXReceived(unsigned char* pbuff, int count) {
if (count > CHANNEL_COUNT) count = CHANNEL_COUNT;
byte b = pbuff[113]; //DMX Subnet
if ( b == DMX_SUBNET) {
b = pbuff[114]; //DMX Universe
if ( b >= DMX_UNIVERSE && b <= DMX_UNIVERSE + UNIVERSE_COUNT ) {
if ( pbuff[125] == 0 ) { //start code must be 0
int dmxChannel = (b - DMX_UNIVERSE) * CHANNELS_PER_UNIVERSE;
// sACN packets come in seperate RGB but we have to set each led's RGB value together
// this 'reads ahead' for all 3 colours before moving to the next led.
//Serial.println("*");

//send buffered DMX universe 1 data to udpLoop
udpLoop(pbuff);

}
}
}
}


int checkACNHeaders(unsigned char* messagein, int messagelength) {
//Do some VERY basic checks to see if it's an E1.31 packet.
//Bytes 4 to 12 of an E1.31 Packet contain "ACN-E1.17"
//Only checking for the A and the 7 in the right places as well as 0x10 as the header.
//Technically this is outside of spec and could cause problems but its enough checks for us
//to determine if the packet should be tossed or used.
//This improves the speed of packet processing as well as reducing the memory overhead.
//On an Isolated network this should never be a problem....
if ( messagein[1] == 0x10 && messagein[4] == 0x41 && messagein[12] == 0x37) {
int addresscount = messagein[123] * 256 + messagein[124]; // number of values plus start code
return addresscount -1; //Return how many values are in the packet.
}
return 0;
}
