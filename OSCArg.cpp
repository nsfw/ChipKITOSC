/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#include <stdlib.h>
#include <string.h>
#include "OSCcommon/OSCArg.h"
#include "OSCCommon/OSCMessage.h"

OSCArg::OSCArg(void){
    _typeTag = 0;
    _argData = 0;
    _dataSize = 0;
    _alignmentSize = 0;
}

OSCArg::OSCArg(char _tag){
    _typeTag = _tag;
    _argData = 0;
    _dataSize = 0;
    _alignmentSize = 0;
}

// sfw - debug
#include "Wprogram.h"
#define DUMPVAR(s,v) Serial.print(s); Serial.println(v);
#define DUMPPTR(s,v) Serial.print(s); Serial.println((unsigned int)v,HEX);

void OSCArg::init( char _tag , void *_data , uint16_t _size , bool _packSizeCulc ) {
    _argData = 0;
    _alignmentSize = 0;
    
    _typeTag = _tag;
    _dataSize = _size;
    
    if( _typeTag == kTagBlob )  _dataSize = _size+4;	// include size argument
    else _dataSize = _size;

    // DUMPVAR("arg._typeTag=",_typeTag);
    // DUMPVAR("arg._dataSize=",_dataSize);

	// Note: we could just check if _size%4 == 0... but whatever
    if( _packSizeCulc ) _alignmentSize = WORD_ALIGNMENT(_size);
    else                _alignmentSize = _size;
    
    if( _size == 0 ) return;
    
    // allocate null space based on alignment
    _argData = calloc( 1, _alignmentSize );		// free in OSCArg::flush() (called by ~OSCArg)
    // DUMPPTR("alloc _argData=",_argData);
    // DUMPPTR("alloc _alignmentSize=",_alignmentSize);
    if(_argData){	// allocated
        // and copy our data into it
        memcpy( (uint8_t*)_argData , _data , _size );
    }
}

OSCArg::~OSCArg(void){
    flush();
}

void OSCArg::flush(void){
    if( _argData ){
        // DUMPPTR("free arg =",_argData);
        free(_argData);
    }
    _dataSize = 0;
    _alignmentSize = 0;
    _typeTag = 0;
}



