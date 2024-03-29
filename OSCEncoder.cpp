/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#include <stdlib.h>
#include <string.h>

#include "Wprogram.h"	// sfw - debug

#include "OSCcommon/OSCcommon.h"
#include "OSCCommon/OSCEncoder.h"
#include "OSCCommon/OSCArg.h"

#define DUMPVAR(s,v) Serial.print(s); Serial.print(v);
#define DUMPPTR(s,v) Serial.print( s); Serial.println((unsigned int)v,HEX);

int16_t OSCEncoder::encode( OSCMessage *_newMes ,uint8_t *_binData ){
	
	uint8_t *packStartPtr = _binData;
    	
	// OSC Address(String) -> BIN Encode

    // DUMPPTR("_binData= ",_binData);
    // DUMPPTR("_newMes->oscAddress= ",_newMes->_oscAddress);
    // DUMPPTR("_newMes->oscAdrSize= ",_newMes->_oscAdrSize);

	memcpy( _binData , _newMes->_oscAddress , _newMes->_oscAdrSize );
	
	packStartPtr += _newMes->_oscAdrAlignmentSize;

	// TypeTag(String) -> BIN Encode 
	
	*packStartPtr = ',';
    
    for ( uint8_t i=0 ; i<_newMes->_argsNum ; i++ ){
        packStartPtr[i+1] = _newMes->getArgTypeTag(i);
    }

	packStartPtr += _newMes->_typeTagAlignmentSize;
	
	// Arguments -> BIN Encode 

	for ( uint8_t i=0 ; i < _newMes->_argsNum ; i++ ) {
		switch ( _newMes->getArgTypeTag(i) ) {
        case kTagInt32:
        case kTagFloat:
        case kTagString:
            memcpy( packStartPtr, _newMes->_args[i]->_argData, _newMes->getArgAlignmentSize(i) );
            break;
        default:
            break;
		}
        packStartPtr += _newMes->getArgAlignmentSize(i);
	}
	return 1;
}

