/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 -------- License -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */				   

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "OSCcommon/OSCcommon.h"
#include "OSCCommon/OSCMessage.h"

#include "Wprogram.h"
#define DUMPVAR(s,v) Serial.print(s); Serial.println(v);
#define DUMPPTR(s,v) Serial.print( s); Serial.println((unsigned int)v,HEX);

OSCMessage::OSCMessage(void)
{	
	_oscAddress = NULL;
    _oscAdrSize = 0;
	_oscAdrAlignmentSize = 0;
    _argsNum = 0;
	_argsAlignmentSize = 0;	
	_port = 0;
}

OSCMessage::OSCMessage(const char *_oscAddr)
{
	_argsNum = 0;
	_argsAlignmentSize = 0;	
	_port = 0;
	
	setOSCAddress(_oscAddr);
}

OSCMessage::~OSCMessage(void)
{
	flush();	// release any associated storage
}


void OSCMessage::flush(void){
	// Reset counters and free any associated storage

    if (_oscAdrSize) free(_oscAddress);

    _oscAddress = NULL;
    _oscAdrSize = 0;
	_oscAdrAlignmentSize = 0;

    _typeTagAlignmentSize = WORD_ALIGNMENT(2);	// at least 4 bytes

    for ( uint8_t i = 0 ; i<_argsNum ; i++ ){
        if(_args[i]){
            delete _args[i];
            // note: 	// can't use new and delete w/ new libc
            // ((OSCArg*)_args[i])->~OSCArg();	// explicity call destructor
            // free(_args[i]);					// and free memory
        } else
            Serial.println("~OSCMessage:: null OSCArg*");
    }
    
	_argsNum = 0;
	_argsAlignmentSize = 0;	
}

void OSCMessage::setAddress(uint8_t *_ipAddr , uint16_t _portNum){	
	setIpAddress(_ipAddr);
	_port = _portNum;
}


void OSCMessage::setIpAddress(uint8_t *_ipAddr){
    memcpy( _ip , _ipAddr , 4 ); 
}

uint8_t* OSCMessage::getIpAddress(void){
    return _ip;  
}


void OSCMessage::setPortNumber(uint16_t _portNum){
    _port = _portNum; 
}

uint16_t OSCMessage::getPortNumber(){
    return _port; 
}



int16_t OSCMessage::setOSCAddress(const char *_address){

	_oscAdrSize = (uint16_t)strlen(_address);
	_oscAdrAlignmentSize = WORD_ALIGNMENT(_oscAdrSize+1);	// w/ space for terminal /0
	
	if( _oscAdrSize > kMaxOSCAdrCharactor ){
		flush();
		return -1;
	}
	
	if( _oscAddress != NULL ) free(_oscAddress);
	
	_oscAddress = (char*)calloc(1,_oscAdrSize+1);
	strcpy( _oscAddress , _address) ;
	
	return 1;
}


uint16_t OSCMessage::getMessageSize(void){
    return  _oscAdrAlignmentSize + _typeTagAlignmentSize + _argsAlignmentSize; 
}

uint16_t OSCMessage::getArgAlignmentSize(uint8_t _index){
    return _args[_index]->_alignmentSize;
}

char   *OSCMessage::getOSCAddress(void){
    return _oscAddress; 
}

int16_t OSCMessage::getArgsNum(void){
    return _argsNum; 
}

char    OSCMessage::getArgTypeTag(int16_t _index)   {
    
    if(_index>_argsNum) return -1;
    
    return _args[_index]->_typeTag; 
    
}


void OSCMessage::swap(uint8_t *data1, uint8_t *data2){
    // OSC is BIG ENDIAN both AVR and PIC32 are LITTLE ENDIAN
    data1[0] = data2[3];
    data1[1] = data2[2];
    data1[2] = data2[1];
    data1[3] = data2[0];
}

int16_t OSCMessage::beginMessage(const char *_address){
    flush();						// clear out structure - release storage
    return setOSCAddress(_address);	// e.g. "/foo"
}

int16_t OSCMessage::setArgData(char _type , void *_value , uint16_t _size, bool _enableAlignment){
    // copies data to new memory, with appropriate alignment
    // DUMPVAR("_argsNum=",_argsNum);
    // void *p = malloc(20);
    // if(p){
    //     free(p);
    // } else {
    //     Serial.println("NO MEMORY!!");
    // }

    _args[_argsNum]= new OSCArg( _type , _value , _size , _enableAlignment );

    // note: can't use new with newlibc on chipkit
    // _args[_argsNum] = (OSCArg *) calloc(1, sizeof(OSCArg));
    // _args[_argsNum]->init(_type , _value , _size , _enableAlignment );
    
    // running total of message size
    uint16_t alignSize = _args[_argsNum]->_alignmentSize;
    _argsAlignmentSize += alignSize;

    // and argument count
    _argsNum++;
    
    _typeTagAlignmentSize = WORD_ALIGNMENT( _argsNum + 2 );	// leading ',' and terminal /0
    
    return alignSize;
}

int16_t OSCMessage::addArgInt32(int32_t _value){
    
    if( _argsNum > kMaxArgument ) return -1;
    
    uint8_t tmpValue[4];

    uint8_t *data = (uint8_t*)&_value;
    swap( tmpValue , data );		// AVR and PIC32 are both little endian

    setArgData( kTagInt32 , tmpValue , 4 , false );	
    
    return 1;
}

int32_t OSCMessage::getArgInt32(int16_t _index){
    
    if ( _index > _argsNum ) return -1;
     
    uint32_t tmpValue;		// enforce word aligment
    uint8_t *_bin = (uint8_t*)_args[_index]->_argData;
    swap( (uint8_t *) &tmpValue , _bin );
    
    return tmpValue;
}



#ifdef _USE_FLOAT_

int16_t OSCMessage::addArgFloat(float _value){
    
    if( _argsNum > kMaxArgument ) return -1;
    
    uint8_t tmpValue[4];
    uint8_t *data = (uint8_t*)&_value;
    swap( tmpValue , data );
    
    setArgData( kTagFloat , tmpValue , 4 , false );
     
    return 1;
}

float OSCMessage::getArgFloat(int16_t _index){
    
    if ( _index > _argsNum ) return -1;
    
     
    uint8_t tmpValue[4];
    uint8_t *_bin = (uint8_t*)_args[_index]->_argData;
    swap( tmpValue , _bin ); 
    
    return *(float*)tmpValue;
    
}
#endif



#ifdef _USE_STRING_
int16_t OSCMessage::addArgString(const char* _value){
    
    if (_argsNum > kMaxArgument ) return -1;
    
    // remember we need storage for null terminator
    setArgData( kTagString , (void*)_value , strlen(_value)+1 , true );
        
    return 1;
}

int16_t  OSCMessage::getArgString(int16_t _index , char *_rcvstr){
    
    if ( _index > _argsNum ) return -1;
    
    strcpy( _rcvstr , (char*)_args[_index]->_argData );
    
    return 1;

}
int16_t  OSCMessage::getArgStringSize(int16_t _index){
    
    if ( _index > _argsNum ) return -1;
    
    return _args[_index]->_dataSize+1;
}
#endif

