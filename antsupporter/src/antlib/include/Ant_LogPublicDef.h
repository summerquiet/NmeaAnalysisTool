/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_LOGPUBLICDEF_H
#define ANT_LOGPUBLICDEF_H

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

// Define some constant value
const BYTE  LOG_LOGNAME_LEN     = 10;                       // Log name's length
const BYTE  LOG_LOGINFO_LEN     = 246;                      // Log informtion's length
const XCHAR PMEM_LOG_BUF[]      = XTEXT("PMEM_LOG_BUF");
const DWORD LOG_MAX_NUM         = 0xFFFFFFFF;

/*---------------------------------------------------------------------------*/
// Struct declare

// Log record header information
struct  Ant_LogRecordHeader
{
    DWORD dwRecordNum;  // Log record total count
    DWORD dwNextPos;    // The next log record's position
    DWORD dwLogNum;     // The log record number 
};

// Log record date
struct Ant_LogDate 
{
    WORD wYear;         // Year
    WORD wMonth;        // Month
    WORD wDay;          // Day
    WORD wReserved;     // Reserved
}; 

// Log record time
struct Ant_LogTime 
{
    WORD wHour;         // Hour
    WORD wMin;          // Minute
    WORD wSec;          // Second
    WORD wMSec;         // Millisecond
};

// Log record
struct Ant_LogRecord 
{
    DWORD       dwLogNum;                       // Log number
    CHAR        szLogName[LOG_LOGNAME_LEN];     // Log name 
    CHAR        szLogInfo[LOG_LOGINFO_LEN];     // Log information
    DWORD       dwTickCount;                    // Tick count
    Ant_LogDate cLogDate;                       // Log date 
    Ant_LogTime cLogTime;                       // Log time          
};

// Log name table
const CHAR* const s_szLogNameTable[] = {
    "ANT_FILE"
    "ANT_TEMP_2",
    "ANT_TEMP_3",
    "ANT_TEMP_4",
    "ANT_TEMP_5",
    "ANT_TEMP_6",
    NULL,
};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_LOGPUBLICDEF_H

/*---------------------------------------------------------------------------*/
/* EOF */
