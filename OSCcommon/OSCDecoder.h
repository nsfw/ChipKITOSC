/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- License -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#ifndef OSCDecoder_h
#define OSCDecoder_h

#include "OSCMessage.h"

class OSCDecoder{
public:
	static int16_t decode( OSCMessage *_mes ,const uint8_t *_binData );
};

#endif
