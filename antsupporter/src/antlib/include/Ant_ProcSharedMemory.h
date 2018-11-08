/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_PROCSHAREDMEMORY_H
#define ANT_PROCSHAREDMEMORY_H
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
#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif
#ifndef ANT_LIST_H
#   include "Ant_List.h"
#endif
#ifndef ANT_STRING_H
#   include "Ant_String.h"
#endif
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class delare
/**
 * @brief class represents a memory block shared between processes.
 *
 * @detail helper class to allocate/deallocate/operate on shared memory.
 */
class ANTLIB_API Ant_ProcSharedMemory : public virtual Ant_Object
{
public:

    Ant_ProcSharedMemory();
    Ant_ProcSharedMemory(const XCHAR* name, LONG size, BOOL clr_flg = FALSE);
    virtual ~Ant_ProcSharedMemory();

    BYTE* alloc(const XCHAR* name, LONG size);
    BYTE* calloc(const XCHAR* name, LONG size);
    VOID free();

    BYTE* getAddress() const {return addr;}
    LONG getSize() const {return memory_size;}

    BYTE* open(const XCHAR* name) {return open(name, 0);}
    BYTE* open(const XCHAR* name, LONG size);
    VOID close();

    VOID clear();
    BOOL read(LONG offset, BYTE* buff, LONG size);
    BOOL write(LONG offset, const BYTE* data, LONG size);

    inline static DWORD getTotalSharedMemCount() {return total_sharedmem_count;}
    inline static DWORD getTotalSharedMemSize() {return total_sharedmem_size;}

private:

    VOID freeMemory();

    static volatile DWORD   total_sharedmem_count;
    static volatile DWORD   total_sharedmem_size;
    static Ant_SyncObj      measure_sync;

#if defined(_WIN32) || defined(_WIN32_WCE)
    HANDLE                  handle;
#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
    //only supoort memory name in one process!!!
    struct ProcMemInfo
    {
        Ant_String strMemName;
        BYTE* byAddr;
        INT nUseCounter;
        DWORD dwSize;
        ProcMemInfo()
        : strMemName()
        , byAddr(NULL)
        , nUseCounter(0)
        , dwSize(0)
        {

        }

        VOID setInfo(const XCHAR* name, DWORD size)
        {
            strMemName.set(name);
            dwSize = size;
        }

        VOID increaseUseCounter()
        {
            if (!byAddr) {
                byAddr = new BYTE[dwSize];
                ANT_PRINTF(("Ant_ProcSharedMemory - malloc(%d) - %p", dwSize, byAddr));
            }
            nUseCounter++;
            ANT_PRINTF(("Ant_ProcSharedMemory (%s)- IncreaseUseCounter %d", strMemName.GetString(), nUseCounter));
        }

        VOID decreaseUseCounter()
        {
            if (nUseCounter > 0) nUseCounter--;
            ANT_PRINTF(("Ant_ProcSharedMemory (%s)- DecreaseUseCounter %d", strMemName.GetString(), nUseCounter));
            if (nUseCounter == 0 && byAddr) {
                ANT_PRINTF(("Ant_ProcSharedMemory - free(%d) - %p", dwSize, byAddr));
                delete[] byAddr;
                byAddr = NULL;
            }
        }
    };
    static Ant_ObjList<ProcMemInfo> s_listAllMemory;
    static Ant_SyncObj              s_listSyncObj;
    ProcMemInfo* m_pcMemInfo;
#elif defined(_LINUX)
    int     m_nFileDes;
#else
    // nothing
#endif
    BYTE*   addr;
    DWORD   memory_size;
    BOOL    created;

private:

    // Disable the copy constructor and operator =
    Ant_ProcSharedMemory(const Ant_ProcSharedMemory& src);
    Ant_ProcSharedMemory& operator = (const Ant_ProcSharedMemory& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_PROCSHAREDMEMORY_H

/*---------------------------------------------------------------------------*/
/* EOF */
