/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */


#ifndef OSCClient_h
#define OSCClient_h

#include "OSCcommon.h"
#include "OSCMessage.h"
#include "OSCEncoder.h"
#include "OSCServer.h"

#define kDummyPortNumber 10000



class OSCClient{
	
// private:
public:     
    uint8_t *_sendData;
    // UDP *udp;
    
    OSCEncoder encoder;

	void flushSendData(void);

    int16_t sockOpen(void);
	void sockClose(void);
    
public:
    
	OSCClient(void);
	~OSCClient(void);
    
    void begin( OSCServer &server ){
        // udp = server.udp;
    };

	int16_t send( OSCMessage *_message);
    
			
};


#endif
