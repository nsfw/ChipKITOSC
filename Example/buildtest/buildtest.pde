// notes for newlibc
// ~/Documents/chipKIT/libraries/chipKITEthernet/utility/TCPIP Stack/Helpers.h
// #define _C32_ULTOA 1
//
// in WCharacter.h: isblank(int c) definition conflicts w/ ctype.h
//

#include <SPI.h>
#include <chipKITEthernet.h>
#include <ArdOSC.h>

// byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myMac[] = { 0, 0, 0, 0, 0, 0 };	// let ethernet library assign something
byte myIp[]  = { 139, 104, 88, 199 };

byte targetIp[]  = { 139, 104, 88, 90 };


int  serverPort  = 9999;

int destPort=9999;

char oscadr[]="/test";

OSCServer server;
OSCClient client;

char blockingReadChar(){
    while(!Serial.available());	// wait for it
    return Serial.read();
}

void dumpMem(){
    int dummy;
    uint32_t diff = 0;
    Serial.println("Mem:");
    void *p = malloc(4);
    if(p){
        Serial.println((unsigned int)&dummy,HEX);
        Serial.println((unsigned int)p, HEX);
        Serial.print("stack/heap diff: ");
        Serial.println((unsigned int) &dummy - (unsigned int) p, HEX);
    } else {
        Serial.println("UNABLE TO ALLOCATE 4 BYTES!");
    }
}

void testAlloc(){
    int p = 0x100;
    void *ptr = 0;
    while(ptr = malloc(p)){
        Serial.print("alloc: ");
        Serial.println(p, HEX);
        Serial.print(" @ ");
        Serial.println((unsigned int) ptr, HEX);
        dumpMem();
        free(ptr);
        delay(1000);
    }
}


void setup(){ 
  
    Serial.begin(57600);
    Serial.println("Hello!!");

    // testAlloc();

    Ethernet.begin(myMac ,myIp); 
 
    server.begin(serverPort);
 
    //set callback function
    server.addCallback(oscadr,&func1);

    dumpMem();
    void * x = malloc(100);
    dumpMem();
    free(x);
    dumpMem();
}
  
void loop(){
    Ethernet.PeriodicTasks();
    Serial.print(".");
    if(server.availableCheck()>0){
         Serial.println("alive! "); 
    }
}


class foo {
    int a;
    int b;
public:

    foo();
};

foo::foo(){
    a=1;
    b=2;
}

void func1(OSCMessage *_mes){
  
    Serial.println("Foo!");

    logIp(_mes);
    logOscAddress(_mes);

    //get source ip address
    byte *sourceIp = _mes->getIpAddress();

    //get 1st argument(int32)
    int tmpI=_mes->getArgInt32(0);
  
    //get 2nd argument(float)
    float tmpF=_mes->getArgFloat(1);
  
    //get 3rd argument(string)
    int strSize=_mes->getArgStringSize(2);
    char tmpStr[strSize]; //string memory allocation
    _mes->getArgString(2,tmpStr); 

  //create new osc message
  OSCMessage newMes;
  
  //set destination ip address & port no
  newMes.setAddress(sourceIp,destPort);

  
  //set argument
  newMes.beginMessage(oscadr);
  uint32_t anint;

  void *p = calloc(1, 20);

  // OSCArg *a = new OSCArg('i' , &anint , 4 , false );	
//   OSCArg b('i' , &foo , 4 , false );
//  OSCServer *s = new OSCServer();
//  foo *f = new foo();
//   OSCServer ss;
//   newMes.setArgData( 'i' , &anint , 4 , false );	

#if 0
  newMes.addArgInt32(tmpI+1);
  newMes.addArgFloat(tmpF+0.1);
  newMes.addArgString(tmpStr);
 
  //send osc message
  client.send(&newMes);
#endif

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
}

void logOscAddress(OSCMessage *_mes){
  Serial.println(_mes->getOSCAddress());
} 
