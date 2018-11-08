/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_ERRORDEF_H
#define ANT_ERRORDEF_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

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

const INT   ERROR_FILEINFO_SIZE     = 88;
const DWORD ERROR_ERRORNUM_MAX      = 0xFFFFFFFF;
const INT   ERROR_MAX_PATH          = MAX_PATH;
const INT   ERROR_DEFAULT_NUM       = 50; 

//error type
enum AntErrorType
{
    ANT_ERROR_NON   = -1,

    ANT_ERROR_ERROR,
    ANT_ERROR_DEBUG,
    ANT_ERROR_FATAL,

    //type number
    ANT_ERROR_TYPE_NUM
};

//Error memory state
enum AntErrorMemState
{
    ANT_ERROR_MEM_NOTREAD,
    ANT_ERROR_MEM_GOTTEN,
    ANT_ERROR_MEM_UPDATE
};

//error date
struct ErrorDate 
{
    WORD wYear;
    WORD wMon;
    WORD wDay;
    WORD wPading;
};

//Error time
struct ErrorTime 
{
    WORD wHour;
    WORD wMin;
    WORD wSec;
    WORD wMSec;
};

//Error record
struct ErrorRecord 
{
    DWORD           dwErrorNum;
    CHAR            szFileInfo[ERROR_FILEINFO_SIZE];
    DWORD           dwErrLine;
    LONG            lErrCode;
    DWORD           dwOption;
    ErrorDate       sErrDate;
    ErrorTime       sErrTime;
};

//Error header
struct ErrorHeader
{
    AntErrorMemState    eMemState;
    BOOL                bMaxRecNumFlag;
    DWORD               dwRecordNum;
    DWORD               dwNextPos;
    DWORD               dwErrNum;
};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_ERRORDEF_H

/*---------------------------------------------------------------------------*/
/* EOF */
