/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_SOCKETCONTEXT_H
#define ANT_SOCKETCONTEXT_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * I/F to initialize socket environment.
 *
 * Before using Ant_Socket, you MUST create an global socket context.
 */
class ANTLIB_API Ant_SocketContext : public virtual Ant_Object
{
public:

    static void createContext();
    static void* context();
    static void destroyContext();

private:

    Ant_SocketContext();
    ~Ant_SocketContext();
};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_SOCKETCONTEXT_H

/*---------------------------------------------------------------------------*/
/* EOF */
