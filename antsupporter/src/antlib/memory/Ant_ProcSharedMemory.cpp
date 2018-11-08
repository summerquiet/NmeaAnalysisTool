/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_PROCSHAREDMEMORY_H
#   include "Ant_ProcSharedMemory.h"
#endif
#ifndef ANT_FILEDEF_H
#   include "Ant_FileDef.h"
#endif
#ifndef ANT_AUTOSYNC_H
#   include "Ant_AutoSync.h"
#endif

#if defined(_WIN32) || defined(_WIN32_WCE)
#   include <TCHAR.h>
#else
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <sys/mman.h>
#   include <fcntl.h>
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

volatile DWORD  Ant_ProcSharedMemory::total_sharedmem_count = 0;
volatile DWORD  Ant_ProcSharedMemory::total_sharedmem_size = 0;
Ant_SyncObj     Ant_ProcSharedMemory::measure_sync;

#if defined(_WIN32) || defined(_WIN32_WCE)
#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
Ant_ObjList<Ant_ProcSharedMemory::ProcMemInfo>  Ant_ProcSharedMemory::s_listAllMemory;
Ant_SyncObj                                     Ant_ProcSharedMemory::s_listSyncObj;
#elif defined(_LINUX)
#else
#endif

/*---------------------------------------------------------------------------*/
// Functions
Ant_ProcSharedMemory::Ant_ProcSharedMemory()
#if defined(_WIN32) || defined(_WIN32_WCE)
: handle(NULL)
#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
: m_pcMemInfo(NULL)
#elif defined(_LINUX)
: m_nFileDes(0)
#else
#endif
, addr(NULL)
, memory_size(0)
, created(FALSE)
{

}

Ant_ProcSharedMemory::Ant_ProcSharedMemory(const XCHAR* name, LONG size, BOOL clr_flg)
#if defined(_WIN32) || defined(_WIN32_WCE)
: handle(NULL)
#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
: m_pcMemInfo(NULL)
#elif defined(_LINUX)
: m_nFileDes(0)
#else
#endif
, addr(NULL)
, memory_size(0)
, created(FALSE)
{
    if (clr_flg) {
        calloc(name, size);
    }
    else {
        alloc(name, size);
    }
}

Ant_ProcSharedMemory::~Ant_ProcSharedMemory()
{
    freeMemory();
}

VOID Ant_ProcSharedMemory::freeMemory()
{
    if (addr) {
        if (created == TRUE) {
            measure_sync.syncStart();
            total_sharedmem_count--;
            total_sharedmem_size -= memory_size;
            measure_sync.syncEnd();
        }
#if defined(_WIN32) || defined(_WIN32_WCE)
        ::UnmapViewOfFile(addr - sizeof(LONG));
#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
        Ant_AutoSync _sync(s_listSyncObj);
        if (m_pcMemInfo) {
            m_pcMemInfo->decreaseUseCounter();
            if (m_pcMemInfo->nUseCounter == 0) {
                Ant_List<ProcMemInfo>::Iterator it = s_listAllMemory.begin();
                do {
                    if (m_pcMemInfo == it.current()) {
                        ANT_PRINTF(("Ant_ProcSharedMemory::FreeMemory() delete [%p].", m_pcMemInfo));
                        it.removeCurrent();
                    }
                } while (NULL != it());
            }
        }
#elif defined(_LINUX)
        munmap(addr - sizeof(LONG),memory_size + sizeof(LONG));
#else
#endif
        addr = NULL;
    }
#if defined(_WIN32) || defined(_WIN32_WCE)
    if (handle) {
        ::CloseHandle(handle);
        handle = NULL;
    }
#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
#elif defined(_LINUX)
    if (m_nFileDes > 0)
    {
        ANT_CLOSE(m_nFileDes);
        m_nFileDes = 0;
    }
#else
#endif

    memory_size = 0;
    created        = FALSE;
}

BYTE* Ant_ProcSharedMemory::alloc(const XCHAR* name, LONG size)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    //if already get the memory handle, free it and realloc.
    if (handle) {
        freeMemory();
    }
    handle = ::CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, size + sizeof(LONG), name);
    //if get memory handle succeed and not EXIST, get the address.
    if (handle) {
        DWORD dwLastError = GetLastError();
        //if the memory already created, return NULL.
        if (dwLastError == ERROR_ALREADY_EXISTS) {
            ::CloseHandle(handle);
            handle = NULL;
            return NULL;
        }
        //get the memory address.
        if ((addr = (BYTE*)::MapViewOfFile(handle, FILE_MAP_WRITE, 0, 0, 0)) == NULL) {
            ::CloseHandle(handle);
            handle = NULL;
            //TODO: memory shortage error.
        }
    }
    else {
        //TODO; get memory handle failed.
    }

#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
    //if already get the address, free it and realloc.
    if (addr) {
        freeMemory();
    }

    {//create new memory
        Ant_AutoSync _sync(s_listSyncObj);
        Ant_List<ProcMemInfo>::Iterator it = s_listAllMemory.begin();
        do {
            ProcMemInfo* cur = it.current();
            if (cur && cur->strMemName == Ant_String(name)) {
                ANT_PERROR("Ant_ProcSharedMemory::Alloc(%s, %ld) already exist.", name, size);
                return NULL; //alrday exist
            }
        }
        while (NULL != it());

        ProcMemInfo& cMemInfo = s_listAllMemory.append();
        cMemInfo.setInfo(name, size+sizeof(LONG));
        cMemInfo.increaseUseCounter();
        m_pcMemInfo = &cMemInfo;
        addr = cMemInfo.byAddr;
        ANT_PRINTF(("Ant_ProcSharedMemory::Alloc(%s, %d) append new [%p].", name, size, m_pcMemInfo));
    }
#elif defined(_LINUX)
    if (m_nFileDes>0) {
        freeMemory();
    }

    {
        //create new memory
        m_nFileDes = ANT_OPEN(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
        if (m_nFileDes < 0) {
             ANT_PRINTF("Ant_ProcSharedMemory::Alloc open file failed.");

            return NULL;
        }
        ANT_LSEEK(m_nFileDes, size + sizeof(LONG), SEEK_SET);
        ANT_WRITE(m_nFileDes, "\0", 1);
        if (ANT_LSEEK(m_nFileDes, 0, SEEK_END) < static_cast<INT>(size)) {
            ANT_PRINTF("Ant_ProcSharedMemory::Alloc lseek file failed.");
            return NULL;
        }
        addr = static_cast<BYTE*>(mmap(
            0,
            size + sizeof(LONG),
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            m_nFileDes,
            0));
        if  (NULL == addr) {
            ANT_PRINTF("Ant_ProcSharedMemory::Alloc mmap file failed.");
            return NULL;
        }
        memset(addr, 0, size + sizeof(LONG));
        ANT_PRINTF(("Ant_ProcSharedMemory::Alloc(%s, %d)", name, size));
    }
#else
    addr = NULL;
#endif

    if (addr) {
        memcpy(addr, &size, sizeof(LONG));
        memory_size = size;
        addr += sizeof(LONG);

        created = TRUE;
        measure_sync.syncStart();
        total_sharedmem_count++;
        total_sharedmem_size += memory_size;
        measure_sync.syncEnd();
    }
    return addr;
}

BYTE* Ant_ProcSharedMemory::calloc(const XCHAR* name, LONG size)
{
    if (alloc(name, size)) {
        memset(addr, 0, memory_size);
    }
    return addr;
}

VOID Ant_ProcSharedMemory::free()
{
    freeMemory();
}

BYTE* Ant_ProcSharedMemory::open(const XCHAR* name, LONG size)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    //if already get the memory handle, free it and realloc.
    if (handle) {
        freeMemory();
    }

    handle = ::CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, 256, name);
    //if get memory handle succeed and already EXIST, get the address.
    if (handle) {
        DWORD dwLastError = GetLastError();
        //if the memory not created, return NULL.
        if (dwLastError != ERROR_ALREADY_EXISTS) {
            ::CloseHandle(handle);
            handle = NULL;
            return NULL;
        }
        //get the memory address.
        if ((addr = (BYTE*)::MapViewOfFile(handle, FILE_MAP_WRITE, 0, 0, 0)) == NULL) {
            ::CloseHandle(handle);
            handle = NULL;
            //TODO: memory open failed.
        }
    }
    else {
        //TODO; get memory handle failed.
    }
#elif defined(_FOR_APPLE_) || defined(_FOR_ANDROID_)
    {//find memory
        Ant_AutoSync _sync(s_listSyncObj);
        Ant_List<ProcMemInfo>::Iterator it = s_listAllMemory.begin();
        do {
            ProcMemInfo* cur = it.current();
            if (cur && cur->strMemName == Ant_String(name)) {
                m_pcMemInfo = cur;
                addr = m_pcMemInfo->byAddr;
                m_pcMemInfo->increaseUseCounter();
                ANT_PRINTF(("Ant_ProcSharedMemory::Open(%s) find the one [%p].", name, m_pcMemInfo));
                break;
            }
        } while (NULL != it());
    }
    if (!m_pcMemInfo) {
        ANT_PRINTF(("Ant_ProcSharedMemory::Open(%s) cann't find the one.", name));
    }
#elif defined(_LINUX)
    if (0 >= size) {
        return addr;
    }
    //if already get the memory handle, free it and realloc.
    if (m_nFileDes>0) {
        freeMemory();
    }

    //create new memory
    m_nFileDes = ANT_OPEN(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (m_nFileDes < 0) {
         ANT_PRINTF("Ant_ProcSharedMemory::Open open file failed.");
        return NULL;
    }
    ANT_LSEEK(m_nFileDes, size + sizeof(LONG), SEEK_SET);
    BYTE data = 0;
    write(m_nFileDes, &data, 1);
    if (ANT_LSEEK(m_nFileDes, 0, SEEK_END) < static_cast<INT>(size)) {
        ANT_PRINTF("Ant_ProcSharedMemory::Open lseek file failed.");
        return NULL;
    }
    addr = static_cast<BYTE*>(mmap(
        0,
        size + sizeof(LONG),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        m_nFileDes,
        0));
    if  (NULL == addr) {
        ANT_PRINTF("Ant_ProcSharedMemory::Open mmap file failed.");
        return NULL;
    }
    //memset(addr, 0, size + sizeof(LONG));
    ANT_PRINTF(("Ant_ProcSharedMemory::Open(%s, %d)", name, size));
#else
    addr = NULL;
#endif

    if (addr) {
        memcpy(&memory_size, addr, sizeof(LONG));
        addr += sizeof(LONG);
    }

    created = FALSE;

    return addr;
}

VOID Ant_ProcSharedMemory::close()
{
    freeMemory();
}

VOID Ant_ProcSharedMemory::clear()
{
    BYTE* baseAddr = getAddress();
    LONG baseSize = getSize();
    if (baseAddr && baseSize > 0) {
        memset(baseAddr, 0, baseSize);
    }
}

BOOL Ant_ProcSharedMemory::read(LONG offset, BYTE* buff, LONG size)
{
    if ((buff == NULL) || (offset < 0) || (size <= 0)) {
        return FALSE;
    }
    LONG baseSize = getSize();
    if ((offset + size) > baseSize) {
        return FALSE;
    }
    BYTE* baseAddr = getAddress();
    if (baseAddr) {
        memcpy(buff, baseAddr + offset, size);
        return TRUE;
    }
    return FALSE;
}

BOOL Ant_ProcSharedMemory::write(LONG offset, const BYTE* data, LONG size)
{
    if ((data == NULL) || (offset < 0) || (size <= 0)) {
        return FALSE;
    }
    LONG baseSize = getSize();
    if ((offset + size) > baseSize) {
        return FALSE;
    }
    BYTE* baseAddr = getAddress();
    if (baseAddr) {
        memcpy(baseAddr + offset, data, size);
        return TRUE;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
