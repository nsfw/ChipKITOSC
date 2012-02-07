/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#include <stdlib.h>
#include <string.h>
#include "OSCcommon/OSCServer.h"
#include "OSCcommon/OSCcommon.h"
#include "OSCCommon/OSCMessage.h"

// work with chipKIT Network Shield


OSCServer::OSCServer(void){
    // _sock = MAX_SOCK_NUM;
    udp = 0;
    decodeErrorCount=0;
}

OSCServer::~OSCServer(void){
    // typically never called...
    stop();
}

int16_t OSCServer::begin(uint16_t _recievePort){
    
	// if ( _sock != MAX_SOCK_NUM ) return -1;
    
    // for (int i = 0 ; i < MAX_SOCK_NUM ; i++) {
    //     uint8_t s = W5100.readSnSR(i);
    //     if ( s == SnSR::CLOSED || s == SnSR::FIN_WAIT ) {
    //         _sock = i;
    //         break;
    //     }
    // }
    
    // if (_sock == MAX_SOCK_NUM) return -1;
	// rcvFlush();
    
    if(!udp) udp = new UDP();
    _port=_recievePort;
    udp->begin(_port);

    return 1;
}


void OSCServer::stop(void){

    // if (_sock == MAX_SOCK_NUM) return;
    // close(_sock);
    // _sock = MAX_SOCK_NUM;
    if(udp) udp->stop();
}

int16_t OSCServer::availableCheck(void){

	// if( !( W5100.readSnIR(_sock) && SnIR::RECV ) ) return -1;
    // if( W5100.getRXReceivedSize(_sock) == 0 ) return -1;

    int packetSize = udp->available();	// note: includes UDP header
    if(!packetSize) return 0;
    
	// need to use results prior to exiting this stack frame
    OSCMessage rcvMes;
    
    // if ( recvfrom( _sock , _rcvData , 1 , rcvMes._ip , &rcvMes._port ) > kMaxRecieveData ) return -1;

    udp->readPacket(&_rcvData[0], (uint16_t) kMaxRecieveData, &rcvMes._ip[0], &rcvMes._port);
    // Serial.print("RX PORT ");
    // Serial.println(rcvMes._port);
    
    // note: this could be a static member function call, there is no decoder
    // specific storage allocated
	if( _decoder.decode( &rcvMes ,_rcvData ) < 0 ){
        decodeErrorCount++;
        return -1;
    }
    
    // dispatch message if it matches a pattern
    _adrMatch.paternComp(&rcvMes);

	return 1;
}


void OSCServer::rcvFlush(void){
    
	// uint8_t byte;
    
    // while ( W5100.getRXReceivedSize(_sock) ) recv( _sock , &byte , 1);
    
}


void OSCServer::addCallback(char *_adr , Pattern::AdrFunc _func ){
    // "*" matches all messages
    _adrMatch.addOscAddress( _adr , _func );
}

