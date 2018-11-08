/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_HEAPMEMORY_H
#define ANT_HEAPMEMORY_H
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
#ifndef ANT_ERRORCODEDEF_H
#   include "Ant_ErrorCodeDef.h"
#endif
#ifndef ANT_LIST_H
#   include "Ant_List.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

const DWORD ANT_HEAPMEM_MAXSIZE = 507 * 1024;    //507KB

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * @brief A Class to wrap Heap operations
 *
 */
class ANTLIB_API Ant_HeapMemory : public virtual Ant_Object
{
public:

    Ant_HeapMemory();
    virtual ~Ant_HeapMemory();

    /**
     * @retval ANT_ERR_NO_ERROR (0)             : no error
     */
    ANT_ERROR create(DWORD initialSize, DWORD maximumSize, BOOL allocSizeLimiter = TRUE);

    ANT_ERROR destroy();

    LPVOID alloc(DWORD bytes);

    ANT_ERROR free(LPVOID mem);

    LPVOID reAlloc(LPVOID mem, DWORD bytes);

    DWORD getLastError() {return m_lastError;};

private:

    DWORD getAvailPhyMem();

    HANDLE          m_hHeap;            // heap handler
#if !defined(_WIN32) && !defined(_WIN32_WCE)
    Ant_List<VOID>   m_memList;          // Memory block list
#endif
    DWORD           m_lastError;        // Last Error
    BOOL            m_allocSizeLimiter; // Limit the Alloc/ReAlloc size

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_HEAPMEMORY_H

/*---------------------------------------------------------------------------*/
/* EOF */
