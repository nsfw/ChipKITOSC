/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#include <stdlib.h>

#include <chipKITEthernet.h>
#include "OSCCommon/OSCClient.h"

// #include <utility/socket.h>
// #include <utility/w5100.h>

OSCClient::OSCClient(void){
    // _sock = MAX_SOCK_NUM;   
    udp = 0;
    _sendData = 0;
}

OSCClient::~OSCClient(void){
    flushSendData();
}

int16_t OSCClient::sockOpen(void){

    // if ( _sock != MAX_SOCK_NUM ) return -1;
    
    // for ( int i = 0 ; i < MAX_SOCK_NUM ; i++ ) {
    //     uint8_t s = W5100.readSnSR(i);
    //     if ( s == SnSR::CLOSED || s == SnSR::FIN_WAIT ) {
    //         _sock = i;
    //         break;
    //     }
    // }
    
    // if ( _sock == MAX_SOCK_NUM ) return -1;
    
	// socket( _sock, SnMR::UDP , kDummyPortNumber , 0 );

    return 1;
}

void OSCClient::sockClose(void){
    
    // if ( _sock == MAX_SOCK_NUM ) return;
    
    // close(_sock);
    
    // _sock = MAX_SOCK_NUM;

}

int16_t OSCClient::send(OSCMessage *_message){
	
    uint16_t result=0;

    // Serial.print("_sendData="); Serial.println((unsigned int) _sendData, HEX);

    flushSendData();	// free send buffer prior to re-allocating

	_sendData = ( uint8_t*) calloc( 1, _message->getMessageSize() );

    // Serial.print("_sendData="); Serial.println((unsigned int) _sendData, HEX);

    if(!_sendData){	// memory error
        Serial.print("ERROR - OSCClient::send - unable to allocate ");
        Serial.println(_message->getMessageSize());
        return -1;
    }

    // Serial.print("msgsize = "); Serial.println(_message->getMessageSize());
    
	if( encoder.encode( _message , _sendData ) < 0 ){
		flushSendData();
        Serial.println("OSCClient:: encode failed!");
		return -1;
	}
    
    // if( sockOpen()<0 ) return -1; //socket open check
	// result = sendto( _sock , _sendData , _message->getMessageSize() , _message->getIpAddress(), _message->getPortNumber() );
	// sockClose();

    // chipkit32 
    if(!udp) udp = new UDP();
    result = udp->sendPacket(_sendData , _message->getMessageSize() , _message->getIpAddress(), _message->getPortNumber() );

    // release our send buffer
    flushSendData();
	return result;
}

void OSCClient::flushSendData(void){
    if(_sendData) free(_sendData);
	_sendData=0;
}



