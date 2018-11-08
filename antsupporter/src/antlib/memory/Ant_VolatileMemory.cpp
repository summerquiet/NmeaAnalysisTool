/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_VOLATILEMEMORY_H
#    include "Ant_VolatileMemory.h"
#endif
#ifndef ANT_FILEDEF_H
#   include "Ant_FileDef.h"
#endif
#ifndef ANT_DEBUG_H
#    include "Ant_Debug.h"
#endif
#if defined(_WIN32) || defined(_WIN32_WCE)
#   include <TCHAR.h>
#elif defined(_LINUX)
#    include <sys/mman.h>
#    include <fcntl.h>
#    include <sys/types.h>
#    include <sys/stat.h>
#    include <sys/sem.h>
#    include <sys/ipc.h>
#    include <sys/shm.h>
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#if defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
Ant_List<Ant_VolatileMemory::MemoryInfo> Ant_VolatileMemory::s_cMemoryInfoList;
#elif defined(_LINUX)
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun {
    int val;                    /* Value for SETVAL */
    struct semid_ds *buf;       /* Buffer for IPC_STAT, IPC_SET */
    struct seminfo    *__buf;   /* Buffer for IPC_INFO*/
};
#endif
#endif

#define KB  *1024
#define MEM_NAME_LEN 80

Ant_VolatileMemory::VMNode Ant_VolatileMemory::s_cNode[] = {
    { XTEXT("VMEM_ERROR_ERROR"),     0 KB },
    { XTEXT("VMEM_ERROR_DEBUG"),     0 KB },
    { XTEXT("VMEM_ERROR_FATAL"),     0 KB },
    { XTEXT("VMEM_LOG_BUF"),         0 KB },
    { XTEXT("VMEM_MAX"),             0 KB }
};

/*---------------------------------------------------------------------------*/
// Functions
Ant_VolatileMemory::Ant_VolatileMemory(VolatileMemoryID eMemID, Ant_String strName, DWORD dwSize)
: m_eMemID(eMemID)
, m_pAddress(NULL)
, m_dwSize(dwSize)
#if defined(_WIN32) || defined(_WIN32_WCE)
, m_hFileHandle(NULL)
, m_pViewOfFile(NULL)
, m_hDataLock(NULL)
#elif defined(_LINUX)
, m_nSemID(-1)
, m_nFileDes(0)
#endif
, m_cMemoryName(strName)
{
    initialize();
}

Ant_VolatileMemory::~Ant_VolatileMemory()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    if (m_pViewOfFile != NULL) {
        UnmapViewOfFile(m_pViewOfFile);
        m_pViewOfFile = NULL;
    }
    if (m_hFileHandle!= NULL) {
        CloseHandle(m_hFileHandle);
        m_hFileHandle = NULL;
    }
    if(m_hDataLock != NULL) {
        ::CloseHandle(m_hDataLock);
        m_hDataLock = NULL;
    }
#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
    Ant_List<MemoryInfo>::Iterator it = s_cMemoryInfoList.begin();
    MemoryInfo* pExistMem = NULL;
    do {
        if (NULL != (pExistMem = it.current())) {
            if (pExistMem->m_strName == m_cMemoryName) {
                pExistMem->m_dwUseCount --;
                ANT_PRINTF(("Ant_VolatileMemory - free %p, usecount %d", pExistMem->m_pAddr, pExistMem->m_dwUseCount));
                if (pExistMem->m_dwUseCount == 0) {
                    it.removeCurrent();
                    ANT_PRINTF(("Ant_VolatileMemory - free %p", pExistMem->m_pAddr));
                    delete[] pExistMem->m_pAddr;
                    delete pExistMem;
                }
                break;
            }
        }
    } while(NULL != it());
#elif defined(_LINUX)
    delete_Sem();

    munmap(m_pAddress,m_dwSize);
    ANT_CLOSE(m_nFileDes);
    m_nFileDes = 0;
#endif

    m_pAddress = NULL;
}

BYTE* Ant_VolatileMemory::getAddress(VOID) const
{
    return m_pAddress;
}

DWORD Ant_VolatileMemory::getSize(VOID) const
{
    return m_dwSize;
}

BOOL Ant_VolatileMemory::synchronizeStart(VOID)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    if  (NULL == m_hDataLock) {
        return FALSE;
    }
    ::WaitForSingleObject(m_hDataLock, INFINITE);

#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
    m_cSyncObj.syncStart();

#elif defined(_LINUX)
    struct sembuf sops={0, -1,SEM_UNDO}; //SEM_UNDO  //IPC_NOWAIT
    INT ret = -1;
    while  (((ret= semop(m_nSemID, &sops, 1))==-1) && (errno==EINTR) );
    if  (-1 == ret) {
        INT error = errno;
        ANT_PRINTF(("\n****SynchronizeStart::errno = %d****\n", error));
        return FALSE;
    }

#endif
    return TRUE;
}

BOOL Ant_VolatileMemory::synchronizeEnd(VOID)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    if  (NULL == m_hDataLock) {
        return FALSE;
    }
    ::ReleaseMutex(m_hDataLock);

#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
    m_cSyncObj.syncEnd();

#elif defined(_LINUX)
    struct sembuf sops={0, +1,SEM_UNDO};  //SEM_UNDO  //IPC_NOWAIT
    INT ret = -1;
    while  (((ret= semop(m_nSemID, &sops, 1))==-1) && (errno==EINTR) );
    if  (-1 == ret ) {
        INT error = errno;
        ANT_PRINTF(("\n****SynchronizeEnd::errno = %d****\n", error));
        return FALSE;
    }

#endif
    return TRUE;
}

BOOL Ant_VolatileMemory::initialize()
{
    if  (m_eMemID < 0 || m_eMemID >= VMEM_MAX) {
        return FALSE;
    }
    if  (0 == m_dwSize) {
        m_dwSize = s_cNode[m_eMemID].m_dwSize;
    }
    if  (XTEXT("") == m_cMemoryName) {
        m_cMemoryName = s_cNode[m_eMemID].m_szName;
    }
    else {
        m_cMemoryName.upperCase();
    }
#if defined(_WIN32) || defined(_WIN32_WCE)
    TCHAR lockName[MEM_NAME_LEN];
    TCHAR memoryName[MEM_NAME_LEN];
    memset(lockName, 0, MEM_NAME_LEN*sizeof(TCHAR));
    memset(memoryName, 0, MEM_NAME_LEN*sizeof(TCHAR));
    Ant_String strTmp = m_cMemoryName + VOLATILE_MEMORY_BUF;
    strTmp.getString(memoryName, MEM_NAME_LEN);
    strTmp = m_cMemoryName + VOLATILE_MEMORY_SYNC;
    strTmp.getString(lockName, MEM_NAME_LEN);

    m_hDataLock = ::CreateMutex(NULL, FALSE, lockName);

    m_hFileHandle = ::CreateFileMapping(
        INVALID_HANDLE_VALUE,            // handle to file
        NULL,                            // security
        PAGE_READWRITE,                  // protection
        0,                               // high-order DWORD of size
        m_dwSize,                        // low-order DWORD of size
        memoryName                       // object name
        );

    if (NULL == m_hFileHandle ) {
        ::ReleaseMutex(m_hDataLock);
        ::CloseHandle(m_hDataLock);
        m_hDataLock = NULL;
        return FALSE;
    }

    m_pViewOfFile = ::MapViewOfFile(
        m_hFileHandle,                   // handle to file-mapping object
        FILE_MAP_ALL_ACCESS,             // access mode
        0,                               // high-order DWORD of offset
        0,                               // low-order DWORD of offset
        0                                // number of bytes to map
        );
    if (m_pViewOfFile == NULL )
    {
        ::ReleaseMutex(m_hDataLock);
        ::CloseHandle(m_hDataLock);
        m_hDataLock = NULL;
        return FALSE;
    }
    m_pAddress = static_cast<BYTE*>(m_pViewOfFile);
    memset(m_pAddress, 0, m_dwSize);
    ::ReleaseMutex(FALSE);

#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
    Ant_List<MemoryInfo>::Iterator it = s_cMemoryInfoList.begin();
    MemoryInfo* pExistMem = NULL;
    do {
        if (NULL != (pExistMem = it.current())) {
            if (pExistMem->m_strName == m_cMemoryName) {
                m_pAddress = pExistMem->m_pAddr;
                pExistMem->m_dwUseCount ++;
                ANT_PRINTF(("Ant_VolatileMemory - open %p, usecount %d", pExistMem->m_pAddr, pExistMem->m_dwUseCount));
                break;
            }
        }
    } while(NULL != it());

    if (m_pAddress == NULL) {
        m_pAddress = new BYTE[m_dwSize];
        memset(m_pAddress, 0, m_dwSize);
        MemoryInfo* pNewMem = new MemoryInfo;
        pNewMem->m_strName = m_cMemoryName;
        pNewMem->m_pAddr = m_pAddress;
        pNewMem->m_dwUseCount = 1;
        ANT_PRINTF(("Ant_VolatileMemory - create %p, usecount %d", pNewMem->m_pAddr, pNewMem->m_dwUseCount));
        s_cMemoryInfoList.add(pNewMem);
    }

#elif defined(_LINUX)
    char lockName[MEM_NAME_LEN];
    char memoryName[MEM_NAME_LEN];
    memset(lockName, 0, MEM_NAME_LEN*sizeof(char));
    memset(memoryName, 0, MEM_NAME_LEN*sizeof(char));
    Ant_String strTmp = m_cMemoryName + VOLATILE_MEMORY_BUF;
    strTmp.getString(memoryName, MEM_NAME_LEN);
    strTmp = m_cMemoryName + VOLATILE_MEMORY_SYNC;
    strTmp.getString(lockName, MEM_NAME_LEN);

    m_nFileDes = ANT_OPEN(memoryName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (m_nFileDes < 0) {
        return FALSE;
    }
    ANT_LSEEK(m_nFileDes, m_dwSize+1, SEEK_SET);
    write(m_nFileDes, "\0", 1);
    if (ANT_LSEEK(m_nFileDes, 0, SEEK_END) < static_cast<INT>(m_dwSize)) {
        return FALSE;
    }
    m_pAddress = static_cast<BYTE*>(mmap(
        0,
        m_dwSize,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        m_nFileDes,
        0));
    if  (NULL == m_pAddress) {
        return FALSE;
    }
    memset(m_pAddress, 0, m_dwSize);

    // Create semaphore
    key_t  sync_key;
    sync_key = ftok(memoryName, 0);
    if  (-1 == sync_key) {
        return FALSE;
    }
    int nRet = create_Sem (sync_key );
    if (nRet == -1) {
        return FALSE;
    }

#endif
    return TRUE;
}

#if defined(_LINUX)
int Ant_VolatileMemory::create_Sem(key_t sync_key)
{
    union semun sem ;
    sem.val=1;
    m_nSemID = semget(sync_key, 1, IPC_CREAT|IPC_EXCL|0666);
    if (m_nSemID == -1) {
        m_nSemID = semget(sync_key, 1, IPC_CREAT|0666);
    }
    else {
        //Initialize Semaphore
        semctl(m_nSemID, 0, SETVAL, sem); //SETVAL : Set initial value according to sem.val
    }
    return m_nSemID;
}

void Ant_VolatileMemory::delete_Sem()
{
    union semun sem ;
    sem.val=0;
    semctl(m_nSemID, 0, IPC_RMID, sem);
}
#endif

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
