/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_OBJECT_H
#define ANT_OBJECT_H
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
// Class delare
/**
 * The root base class of all classes begins with "Ant_".
 *
 * It handles common operations on all objects, such as memory allocation, etc.
 * It's designed to be inherited virtually (via keyword virtual).
 */
class ANTLIB_API Ant_Object
{
public:

    Ant_Object();
    virtual ~Ant_Object();

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_OBJECT_H

/*---------------------------------------------------------------------------*/
/* EOF */
