/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_SOCKETLOG_H
#   include "Ant_SocketLog.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

static char LOG_TAG[] = "SOCKET ";

/*---------------------------------------------------------------------------*/
// Functions
VOID ANT_SOCKET_LOGV(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    // TEST by summer
    // MLC_ILogCore::createInstance()->output(MLC_ILogCore::kLevelDebug, LOG_TAG, format, va);
    va_end(va);
}

VOID ANT_SOCKET_LOGI(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    // TEST by summer
    // MLC_ILogCore::createInstance()->output(MLC_ILogCore::kLevelInfo, LOG_TAG, format, va);
    va_end(va);
}

VOID ANT_SOCKET_LOGE(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    // TEST by summer
    // MLC_ILogCore::createInstance()->output(MLC_ILogCore::kLevelError, LOG_TAG, format, va);
    va_end(va);
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
