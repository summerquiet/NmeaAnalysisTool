/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_SOCKETLOG_H
#define ANT_SOCKETLOG_H

/*---------------------------------------------------------------------------*/
// Include files

#include "Ant_NewTypesDefine.h"

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Function declare
VOID ANT_SOCKET_LOGV(const char* format, ...);
VOID ANT_SOCKET_LOGI(const char* format, ...);
VOID ANT_SOCKET_LOGE(const char* format, ...);

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_SOCKETLOG_H

/*---------------------------------------------------------------------------*/
/* EOF */
