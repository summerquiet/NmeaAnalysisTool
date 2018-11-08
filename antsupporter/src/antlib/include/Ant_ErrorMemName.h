/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_ERRORMEMNAME_H
#define ANT_ERRORMEMNAME_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_ERRORDEF_H
#   include "Ant_ErrorDef.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

static const XCHAR* s_pcErrorMemName[ERROR_TYPE_NUM] = {
    XTEXT("ANT_ERR_RECOVERY"),
    XTEXT("ANT_ERR_DEBUG"),
    XTEXT("ANT_ERR_FATAL")};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_ERRORMEMNAME_H

/*---------------------------------------------------------------------------*/
/* EOF */
