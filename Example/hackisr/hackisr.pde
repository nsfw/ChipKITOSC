// notes for newlibc
// ~/Documents/chipKIT/libraries/chipKITEthernet/utility/TCPIP Stack/Helpers.h
// #define _C32_ULTOA 1
//
// in WCharacter.h: isblank(int c) definition conflicts w/ ctype.h
//

#include "exceptions.h"
//#include <SPI.h>
#include <chipKITEthernet.h>
#include <chipKITOSC.h>

#include <peripheral/timer.h>

// byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myMac[] = { 0, 0, 0, 0, 0, 0 };	// let ethernet library assign something
byte myIp[]  = { 139, 104, 88, 199 };

byte targetIp[]  = { 139, 104, 88, 42 };

int  serverPort  = 9999;

int destPort=9999;

char oscadr[]="/test";

OSCServer server;
OSCClient client;

char blockingReadChar(){
    while(!Serial.available());	// wait for it
    return Serial.read();
}

int volatile icnt = 0;

void setup(){ 
  
    Serial.begin(57600);
    Serial.println("Hello!");

    Ethernet.begin(myMac ,myIp); 

    server.begin(serverPort);
    client.begin(server);

    //set callback function
    server.addCallback(oscadr,&func1);

    pinSetup();

    // ISR stuff
    OpenTimer3( T3_ON | T3_PS_1_1 | T3_SOURCE_INT, 0x800);
    ConfigIntTimer3((T3_INT_ON | T3_INT_PRIOR_3 ));
    Serial.println("Note:Using PIC32 Timer 3 ISR");

    // OpenTimer2( T2_ON | T2_PS_1_1 | T2_SOURCE_INT, 0x800);
    // ConfigIntTimer2((T2_INT_ON | T2_INT_PRIOR_3 ));
    // Serial.println("Note:Using PIC32 Timer 2 ISR");

}

extern "C" {
// void __ISR(_TIMER_2_VECTOR,IPL3AUTO) timerISR(void){
//     icnt++;
//     mT2ClearIntFlag();  // Clear interrupt flag
// }
void __ISR(_TIMER_3_VECTOR,IPL3AUTO) timerISR(void){
     icnt++;
     mT3ClearIntFlag();  // Clear interrupt flag
 }

}

void pinSetup(){
    Serial.print("OSCON: ");
    Serial.println(OSCCON,HEX);

#if 0 
	// unlock protected registers
    SYSKEY = 0;		// force lock
    SYSKEY = 0xAA996655;	// unlock sequence
    SYSKEY = 0x556699AA;
    OSCCONCLR = _OSCCON_SOSCEN_MASK;	// turn off Secondary Oscillator
//    OSCCONSET = _OSCCON_SOSCEN_MASK;	// turn ON Secondary Oscillator
    SYSKEY = 0;		// relock
#endif

    Serial.print("OSCON: ");
    Serial.println(OSCCON,HEX);
}

int tobang[]={
    // j3 - 8-13, 79-85 OK
    8,
    9,
    10,
    11,
    12,
    13,
    78,
    79,
    80,
    81,
    82,
    83,
    84,
    85,

    // j4
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,

    // J9
    22,
    23,
    // 24,		// NO VBUS (but supposed to be usable as IO)
    25,
    // 26,		// NO D-/RG3 (pic57)
    // 27,		// NO D+/RG2 (pic56)
    // 28,		// NO AERXERR/RG15 (pic1)
    29,

    30,	// J9
    31,
    32,
    33,
    34,
    35,
    36,
    37,

//    53,	// J8 - Ethernet
    52,
    51,
    50,
    49,
    //   48, 
    47,
    // 46,
    45,
    44,
    // 43,

    39,
    38,

    // J14 (16 - 4)
    // 0 ,	// vcc 
    // 1 ,	// vcc
    2 ,
    3 ,
    4 ,
    5 ,	
    6 ,
	// 7 , Used to reset Ethernet PHY
    70,
    71,
    72,
    73,
    74,
    // 75,	// SOSCI (Secondary Osc Input)
    76,
    77,

    // J7 - "analog in pins" along bottom
    54,
    55,
    56,
    57,
    58,
    59,
    60,
    61,
// 
    62,
    63,
    64,
//    65,	// AN11, 12, 13
//    67,
//    66,
    68,
    69
};

void bangem(){
    static bool on=0;
    static bool firstTime=1;
    for(int i=0; i<(sizeof(tobang)/sizeof(int)); i++){
        int pin = tobang[i];
        if(firstTime) pinMode(pin, OUTPUT);
        digitalWrite(pin, (int) on);
    }
    if(firstTime){
        Serial.print("Pins = ");
        Serial.println(sizeof(tobang)/sizeof(int));
        firstTime=0;
    }
    on = !on;
    delay(1);
};
            
void loop(){
    Serial.print("icnt=");
    Serial.println(icnt);
    TRY {
        Ethernet.PeriodicTasks();
        if(server.availableCheck()){
            Serial.println("alive! "); 
        }
        // makeMsg();	// and send
        // delay(100);
        bangem();
    } CATCH {
      Serial.println("## EXCEPTION ############################################");
      if (_excep_code == EXCEP_Trap) {
          Serial.print("Trap exception: ");
      } else if (_excep_code == EXCEP_DBE) {
          Serial.print("Bus error (load/store) exception: ");
      } else {
          Serial.print("Unknown exception: ");
      }
      Serial.print(_excep_code);
      Serial.println();
      Serial.println("#######################################################");
    }
}

// /test + nothing = /test\0\0\0,\0\0\0\0 = 12 

int cnt = 0;
void makeMsg(){
  //create new osc message
  OSCMessage newMes;
  
  //set destination ip address & port no
  newMes.setAddress(targetIp,destPort);
  
  //set argument
  newMes.beginMessage(oscadr);
  newMes.addArgInt32(cnt);
  newMes.addArgFloat(0.1*cnt);
  newMes.addArgString("HLEOEOEOEO!");

  // Serial.print("s");
  // Serial.print("addrlen: "); Serial.println(newMes._oscAdrAlignmentSize);
  // Serial.print("typelen: "); Serial.println(newMes._typeTagAlignmentSize);

  //send osc message
  client.send(&newMes);
  // Serial.println("!");
  cnt++;

  // chipkit32 will fault on free(0)
  // if(cnt == 10){
  //     Serial.println("BANG?");
  //     free((void*) 0);
  // }
}

void func1(OSCMessage *_mes){
  
    logIp(_mes);
    logOscAddress(_mes);
  
    //get source ip address
    byte *sourceIp = _mes->getIpAddress();
    unsigned int srcPort = _mes->getPortNumber();

    //get 1st argument(int32)
    int tmpI=_mes->getArgInt32(0);
  
    //get 2nd argument(float)
    float tmpF=_mes->getArgFloat(1);
  
    //get 3rd argument(string)
    int strSize=_mes->getArgStringSize(2);

    char tmpStr[strSize]; //string memory allocation
    _mes->getArgString(2,tmpStr); 

    Serial.print(tmpI); Serial.print(" ");
    Serial.print(tmpF); Serial.print(" ");
    Serial.print(tmpStr); Serial.print(" ");
    Serial.println("");

    //create new osc message
    OSCMessage newMes;
  
    //set destination ip address & port no
    newMes.setAddress(sourceIp,srcPort);
  
    //set argument
    newMes.beginMessage(oscadr);
    newMes.addArgInt32(tmpI+1);
    newMes.addArgFloat(tmpF+0.1);
    newMes.addArgString(tmpStr);
 
    //send osc message
    client.send(&newMes);
}

void logIp(OSCMessage *_mes){
  byte *ip = _mes->getIpAddress();
  Serial.print("IP:");
  Serial.print(ip[0],DEC);
  Serial.print(".");
  Serial.print(ip[1],DEC);
  Serial.print(".");
  Serial.print(ip[2],DEC);
  Serial.print(".");
  Serial.print(ip[3],DEC);
  Serial.print(" ");

  Serial.print("Port: ");
  Serial.print(_mes->getPortNumber(),DEC);
  Serial.print(" ");

}

void logOscAddress(OSCMessage *_mes){
  Serial.println(_mes->getOSCAddress());
} 

