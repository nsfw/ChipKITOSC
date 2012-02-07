/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#ifndef ArdOSC_OSCArg_h
#define ArdOSC_OSCArg_h

#include "OSCcommon.h"


class OSCArg{

public:
    char        _typeTag;
    void*       _argData;
    uint16_t    _dataSize;
    uint16_t    _alignmentSize;
    
    OSCArg(void);
    OSCArg(char _tag);
    OSCArg(char _tag, void *_data, uint16_t _size, bool _packSizeCulc){
        init(_tag, _data, _size, _packSizeCulc);
    };

    void init(char _tag, void *_data, uint16_t _size, bool _packSizeCulc);
    
	~OSCArg(void);
	
    void flush(void);
    
	friend class OSCDecoder;
	friend class OSCEncoder;
    friend class OSCMessage;
};


#endif
