/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_ERRORCODEDEF_H
#define ANT_ERRORCODEDEF_H

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
/* 0xFF000000 ~ 0xFF00FFFF */
const ANT_ERROR ANT_ERR_NO_ERROR                   = (0x00000000);   // no error

const ANT_ERROR ANT_ERR_PARAM_ERROR                = (0xFF000000);   // parameter error
const ANT_ERROR ANT_ERR_DIVIDE_BY_ZERO             = (0xFF000001);   // divide by zero

const ANT_ERROR ANT_ERR_HEAPMEM_ERROR_OCCURRED     = (0xFF000011);   // Ant_HeapMemory call API error
const ANT_ERROR ANT_ERR_HEAPMEM_DOUBLE_CREATE      = (0xFF000012);   // Ant_HeapMemory create twice
const ANT_ERROR ANT_ERR_HEAPMEM_NO_HEAP            = (0xFF000013);   // Ant_HeapMemory not create firstly
const ANT_ERROR ANT_ERR_HEAPMEM_SIZE_LIMITED       = (0xFF000015);   // Ant_HeapMemory alloc size is bigger than limited size
const ANT_ERROR ANT_ERR_HEAPMEM_NO_PHYSICS_MEM     = (0xFF000016);   // Ant_HeapMemory no enough physics memory to create heap

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_ERRORCODEDEF_H

/*---------------------------------------------------------------------------*/
/* EOF */
