/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_LOGKINDDEF_H
#define ANT_LOGKINDDEF_H

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

const DWORD ANT_LOG_KIND_ALL            = 0xffffffff;
const DWORD ANT_LOG_KIND_BLOCK1         = 0x00100000;
const DWORD ANT_LOG_KIND_BLOCK2         = 0x00200000;
const DWORD ANT_LOG_KIND_BLOCK3         = 0x00400000;
const DWORD ANT_LOG_KIND_BLOCK4         = 0x00800000;
const DWORD ANT_LOG_KIND_BLOCK5         = 0x01000000;
const DWORD ANT_LOG_KIND_BLOCK6         = 0x02000000;
const DWORD ANT_LOG_KIND_BLOCK7         = 0x04000000;
const DWORD ANT_LOG_KIND_BLOCK8         = 0x08000000;
const DWORD ANT_LOG_KIND_BLOCK9         = 0x10000000;
const DWORD ANT_LOG_KIND_BLOCK10        = 0x20000000;
const DWORD ANT_LOG_KIND_BLOCK11        = 0x40000000;
const DWORD ANT_LOG_KIND_BLOCK12        = 0x80000000;

/// LOG ID for PCORE
/// The block1 is for PCORE, the number of PCORE's LogID is 20.
const DWORD ANT_LOG_KIND_TEST1          = (ANT_LOG_KIND_BLOCK1 | (0x00000001 << (1 - 1)));  // 01 test1
const DWORD ANT_LOG_KIND_TEST2          = (ANT_LOG_KIND_BLOCK1 | (0x00000001 << (2 - 1)));  // 02 test2

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_LOGKINDDEF_H

/*---------------------------------------------------------------------------*/
/* EOF */

