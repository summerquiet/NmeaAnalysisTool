/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_THREADSYSTEM_H
#define ANT_THREADSYSTEM_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_THREADSYSTEMIF_H
#   include "Ant_ThreadSystemIF.h"
#endif
#ifndef ANT_List_H
#   include "Ant_List.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * @brief class the manage all threads.
 *
 * @detail It manages the thread table which records thread name, priority, etc.
 */
class ANTLIB_API Ant_ThreadSystem : public Ant_ThreadSystemIF
{
public:

    Ant_ThreadSystem();
    virtual ~Ant_ThreadSystem();

    virtual INT  registerThread(XCHAR* name, HANDLE handle, DWORD thread_id);

    virtual VOID unregisterThread(INT thread_no);

    virtual BOOL addThreadTable(const ANT_THREAD_TABLE*);

    virtual BOOL getThreadTableInfo(const XCHAR* name, DWORD* priority, DWORD& sanity);

private:

    Ant_List<ANT_THREAD_TABLE>*  m_threadTableList;

private:

    // Disable the copy constructor and operator =
    Ant_ThreadSystem(const Ant_ThreadSystem& src);
    Ant_ThreadSystem& operator = (const Ant_ThreadSystem& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

#endif  // ANT_THREADSYSTEM_H

/*---------------------------------------------------------------------------*/
/* EOF */
