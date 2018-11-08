/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_UTILITYLIB_H
#   include "Ant_UtilityLib.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
VOID reverseByteData(VOID* readAddress, LONG dataSize)
{
    CHAR* dataPtr;
    LONG  dataCount;
    CHAR  tempData;

    dataPtr=(CHAR*)readAddress;
    for( dataCount=0; dataCount< ( dataSize/2 ); dataCount++ ){
    tempData=*dataPtr;
    *dataPtr=*(dataPtr+1);
    *(dataPtr+1)=tempData;
    dataPtr+=2;
    }
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
