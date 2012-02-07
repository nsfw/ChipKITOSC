/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */

#include <stdlib.h>
#include <string.h>

#include "OSCcommon/OSCcommon.h"
#include "OSCcommon/OSCDecoder.h"
#include "OSCcommon/OSCArg.h"

// #include <Wprogram.h>	// sfw

#if 0
#define DUMPVAR(s,v) Serial.print(s); Serial.println((unsigned int) v);
#define DUMPPTR(s,v) Serial.print( s); Serial.println((unsigned int)v,HEX);
void dumphex(byte *buffer,int size){
    Serial.println("dump: ");
    for(byte i = 0; i < size; i++){
        Serial.print((unsigned int) buffer+i,HEX); Serial.print(": ");
        Serial.print((int) buffer[i], HEX);
        Serial.print("  '");
        Serial.println((char) buffer[i]);
    }
}
#endif

int16_t OSCDecoder::decode( OSCMessage *_newMes , const uint8_t *_binData ){
	
    // dumphex((byte *) _binData, 32);
	
	const uint8_t *packetPtr = _binData;
	
	_newMes->beginMessage( (char*)packetPtr );	// address string is null terminated
    
    // Serial.print("packetPtr (start): "); Serial.println((unsigned int)packetPtr,HEX);
    
	packetPtr += _newMes->_oscAdrAlignmentSize;
    
    // Serial.print("packetPtr (typestr): "); Serial.println((unsigned int)packetPtr,HEX);
    
    char *tmpTag = (char*)(packetPtr+1);
    uint8_t argsNum = strlen(tmpTag);
    uint16_t typeTagAlignSize = WORD_ALIGNMENT(argsNum+2);	// count ',' and /0
    
    packetPtr += typeTagAlignSize;
    
    // Serial.print("packetPtr (arg0): "); Serial.println((unsigned int)packetPtr,HEX);
    // Serial.print("argsNum="); Serial.println((int) argsNum);
    // Serial.print("tmpTag="); Serial.println(tmpTag);
    
    for (uint8_t i=0 ; i< argsNum ; i++) {
        
        switch ( tmpTag[i] ) {
        case kTagInt32:
            packetPtr += _newMes->setArgData( kTagInt32 , (void*)packetPtr , 4 , false );
            break;
            
        case kTagFloat:
            packetPtr += _newMes->setArgData( kTagFloat , (void*)packetPtr , 4 , false );
            break;
            
        case kTagString:
        {
            uint16_t l = strlen((char *)packetPtr);
            // DUMPVAR("strlen=", l);
            packetPtr += _newMes->setArgData( kTagString , (void*)packetPtr , l , true );
            break;
        }   
#ifdef _USE_BLOB_
        case kTagBlob:
        {
            uint32_t size;
            uint8_t *tmpPtr=(uint8_t*) &size;		// unpack bigendian size
            *(tmpPtr+3)=*(uint8_t*)packetPtr++;
            *(tmpPtr+2)=*(uint8_t*)packetPtr++;
            *(tmpPtr+1)=*(uint8_t*)packetPtr++;
            *tmpPtr=    *(uint8_t*)packetPtr++;
            // DUMPVAR("bloblen=", size);

            packetPtr += _newMes->setArgData( kTagBlob , (void*)packetPtr , size , true );
            break;
        }
#endif

        default:
            // Serial.print("Decode: WTF?");
            // DUMPVAR("tmpTag[i]", tmpTag[i] );
            // DUMPVAR("i ",i);
            // Serial.println("dump of first 80");
            // dumphex((byte*)_binData,80);
            return -1;	// BAIL!
            break;
        }
    }

	return 1;
}

