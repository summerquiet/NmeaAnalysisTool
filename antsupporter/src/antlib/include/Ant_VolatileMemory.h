/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_VOLATILEMEMORY_H
#define ANT_VOLATILEMEMORY_H
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
#ifndef ANT_STRING_H
#   include "Ant_String.h"
#endif
#if defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
#   include "Ant_SyncObj.h"
#   include "Ant_List.h"
#elif defined(_LINUX)
#   include <pthread.h>
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

enum VolatileMemoryID
{
    VMEM_PARCEL_ID_AREA = 0,
    VMEM_ERROR_ERROR,
    VMEM_ERROR_DEBUG,
    VMEM_ERROR_FATAL,
    VMEM_LOG_BUF,
    VMEM_MAX,
};

const XCHAR VOLATILE_MEMORY_BUF[]   = XTEXT("_BUF");
const XCHAR VOLATILE_MEMORY_SYNC[]  = XTEXT("_SYNC");

/*---------------------------------------------------------------------------*/
// Class declare

/**
 *  class Ant_VolatileMemory
 *
 */
class ANTLIB_API Ant_VolatileMemory : public virtual Ant_Object
{
public:

    Ant_VolatileMemory(VolatileMemoryID eMemID, Ant_String strName = TEXT(""), DWORD dwSize = 0);
    virtual ~Ant_VolatileMemory();

    /**
     * Get the address of the memory block.
     *
     * @return: Address of the memory block.
     */
    BYTE* getAddress(VOID) const;

    /**
     * Get the size of the memory block.
     *
     * @return: Size of the memory block.
     */
    DWORD getSize(VOID) const;

    /**
     * Synchronization interface. Lock the memory block.
     */
    BOOL synchronizeStart(VOID);

    /**
     * Synchronization interface. Unlock the memory block.
     */
    BOOL synchronizeEnd(VOID);

private:

    BOOL initialize();

    struct VMNode
    {
        const XCHAR*    m_szName;
        DWORD           m_dwSize;
    };

    const VolatileMemoryID      m_eMemID;       //Memory ID for Memory Table
    BYTE*                       m_pAddress;     //Memory Base Address
    DWORD                       m_dwSize;       //Memory Size

#if defined(_WIN32) || defined(_WIN32_WCE)
    HANDLE                      m_hFileHandle;  //FileMapping Handle
    LPVOID                      m_pViewOfFile;  //FileMapping Address
    HANDLE                      m_hDataLock;    //Process Syncroni Object
#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
    Ant_SyncObj m_cSyncObj;

    struct MemoryInfo {
        Ant_String  m_strName;
        BYTE*       m_pAddr;
        DWORD       m_dwUseCount;
    };

    static Ant_List<MemoryInfo>  s_cMemoryInfoList;
#elif defined(_LINUX)
    int create_Sem(key_t sync_key);
    void delete_Sem();

    int                         m_nSemID;
    int                         m_nFileDes;
#endif

    Ant_String                  m_cMemoryName;  //FielNapping Name
    static VMNode               s_cNode[];      //Memory Table

private:

    // Disable the copy constructor and operator =
    Ant_VolatileMemory(const Ant_VolatileMemory&);
    Ant_VolatileMemory& operator = (const Ant_VolatileMemory&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_VOLATILEMEMORY_H

/*---------------------------------------------------------------------------*/
/* EOF */
