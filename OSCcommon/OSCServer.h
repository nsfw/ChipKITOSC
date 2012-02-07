/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#ifndef ArdOSC_OSCServer2_h
#define ArdOSC_OSCServer2_h

#include <chipKITEthernet.h>

#include "OSCDecoder.h"
#include "Pattern.h"

class OSCServer{
    
public:
// private:
//     int16_t _sock;

    UDP *udp;

    OSCDecoder _decoder;
    Pattern _adrMatch;
	uint8_t _rcvData[kMaxRecieveData];

    uint16_t _port;
    uint16_t decodeErrorCount;
	
	void rcvFlush(void);		// empty receive queue
public:
    
    OSCServer(void);
	~OSCServer(void);
    
	int16_t begin(uint16_t _recievePort);
	void stop(void);
	
    int16_t availableCheck(void);

    //_adr osc address string pointer - "/ard/aaa"
    //_func callback function pointer
    void addCallback(char *_adr , Pattern::AdrFunc _func );

};

#endif
