/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#ifndef OSCcommon_h
#define OSCcommon_h

extern "C" {
#include <inttypes.h>
}

// 0=0, 1=4, 2=4, 3=4, 4=4
#define WORD_ALIGNMENT(x) (x+3)&0xfffc

//======== user define ==============
#define kMaxArgument	16
#define kMaxRecieveData	2048	// more than a full ethernet mtu
#define kMaxOSCAdrCharactor	255

#define _USE_FLOAT_
#define _USE_STRING_
#define _USE_BLOB_

//======== user define  end  ========




#endif
