/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_UTILITYLIB_H
#define ANT_UTILITYLIB_H
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

/*---------------------------------------------------------------------------*/
// Functions

/*---------------------------------------------------------------------------*/
// Ant_ReverseByteData.cpp
ANTLIB_API VOID ReverseByteData(VOID* readAddress, LONG dataSize);

/*---------------------------------------------------------------------------*/
// Ant_SortLibrary.cpp
ANTLIB_API VOID PbQuickWithInsSort(VOID* CmpInfo, VOID* SortAry, SHORT DataNum, SHORT DataSize, LONG (*FuncCmp)(const VOID*, const VOID*, VOID*));
ANTLIB_API VOID PbShellSort(VOID* CmpInfo, VOID* SortAry, SHORT DataNum, SHORT DataSize, LONG (*FuncCmp)(const VOID*, const VOID*, VOID*));

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_UTILITYLIB_H

/*---------------------------------------------------------------------------*/
/* EOF */
