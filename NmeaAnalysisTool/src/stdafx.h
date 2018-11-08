/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef CXX_STDAFX_H
#define CXX_STDAFX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>

#if defined(_WIN32) || defined(_WIN32_WCE)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Windows

#include <windows.h>

typedef UINT32 uint32_t;

typedef double DOUBLE;

#else //_WIN32 || _WIN32_WCE

#include "Ant_NewTypesDefine.h"
using namespace antsupporter;

#endif //_WIN32 || _WIN32_WCE

#ifndef CXX_NMEADECODEDEFINE_H
#   include "NmeaDecodeDefine.h"
#endif

/*---------------------------------------------------------------------------*/
#endif  // CXX_STDAFX_H

/*---------------------------------------------------------------------------*/
/* EOF */
