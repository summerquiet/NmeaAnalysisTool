/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_HEAPMEMORY_H
#   include "Ant_HeapMemory.h"
#endif
#ifndef ANT_ERROR_H
#   include "Ant_Error.h"
#endif
#include <stdio.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

// static const CHAR g_szErrorFileInfo[] = "$Id: Ant_HeapMemory.cpp $";

/*---------------------------------------------------------------------------*/
// Funcation
Ant_HeapMemory::Ant_HeapMemory()
{
    m_hHeap = 0;
    m_lastError = 0;
    m_allocSizeLimiter = TRUE;
}

Ant_HeapMemory::~Ant_HeapMemory()
{
    destroy();
}

DWORD Ant_HeapMemory::getAvailPhyMem()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    MEMORYSTATUS memStatus;
    memset(&memStatus, 0, sizeof(memStatus));
    memStatus.dwLength = sizeof(MEMORYSTATUS);
    ::GlobalMemoryStatus(&memStatus);
    return memStatus.dwAvailPhys;
#else
    return 0xFFFFFFFF;
#endif
}

ANT_ERROR Ant_HeapMemory::create(DWORD initialSize, DWORD maximumSize, BOOL allocSizeLimiter)
{
    if (m_hHeap != 0) { // 2nd Create
        ErrorLog(ANT_ERR_HEAPMEM_DOUBLE_CREATE, 0);
        return ANT_ERR_HEAPMEM_DOUBLE_CREATE;
    }

    //if allocSizeLimiter is true, maxmumSize must be bigger than zero.
    //otherwise maxmumSize must be zero.
    if (allocSizeLimiter) {
        if (maximumSize == 0) { // invalid parameter
            ErrorLog(ANT_ERR_PARAM_ERROR, maximumSize);
            return ANT_ERR_PARAM_ERROR;
        }
    }
    else {
        maximumSize = 0;
    }

#if defined(_WIN32) || defined(_WIN32_WCE)
    //if create size if bigger than left physical memory, not allowed.
    DWORD avalidPhys = getAvailPhyMem();
    if ((initialSize > avalidPhys) || (maximumSize > avalidPhys)) {
        ErrorLog(ANT_ERR_HEAPMEM_NO_PHYSICS_MEM, avalidPhys);
        return ANT_ERR_HEAPMEM_NO_PHYSICS_MEM;
    }

    m_hHeap = ::HeapCreate(0, initialSize, maximumSize);
    if (m_hHeap == 0) {
        m_lastError = ::GetLastError();
        ErrorLog(ANT_ERR_HEAPMEM_ERROR_OCCURRED, m_lastError);
        return ANT_ERR_HEAPMEM_ERROR_OCCURRED;
    }
#else
    m_hHeap = (HANDLE)-1;
#endif

    m_allocSizeLimiter = allocSizeLimiter;
    return ANT_ERR_NO_ERROR;
}

ANT_ERROR Ant_HeapMemory::destroy()
{
    if (m_hHeap == 0) {
        return ANT_ERR_NO_ERROR;
    }

#if defined(_WIN32) || defined(_WIN32_WCE)
    BOOL ret = FALSE;

    ret = ::HeapDestroy(m_hHeap);

    m_hHeap = 0;
    if (!ret) {
        m_lastError = ::GetLastError();
        ErrorLog(ANT_ERR_HEAPMEM_ERROR_OCCURRED, m_lastError);
        return ANT_ERR_HEAPMEM_ERROR_OCCURRED;
    }
#else
    //free all memorys
    Ant_List<VOID>::Iterator it = m_memList.begin();
    do {
        delete[] it.current();
    } while(NULL != it());
    m_memList.clear();
    m_hHeap = 0;
#endif
    return ANT_ERR_NO_ERROR;
}

LPVOID Ant_HeapMemory::alloc(DWORD bytes)
{
    if (m_hHeap == 0) {
        ErrorLog(ANT_ERR_HEAPMEM_NO_HEAP, 0);
        return 0;
    }

    LPVOID mem = 0;

    //if allocSizeLimiter is true, limit the alloc size.
    //otherwise judge the physics memory size.
    if (m_allocSizeLimiter) {
        if (bytes >= ANT_HEAPMEM_MAXSIZE) {
            ErrorLog(ANT_ERR_HEAPMEM_SIZE_LIMITED, bytes);
            return 0;
        }
    }
#if defined(_WIN32) || defined(_WIN32_WCE)

    mem = ::HeapAlloc(m_hHeap, 0, bytes);
    if (mem == 0) {
        m_lastError = ::GetLastError();
        ErrorLog(ANT_ERR_HEAPMEM_ERROR_OCCURRED, m_lastError);
        return 0;
    }
#else
    mem = new BYTE[bytes];
    m_memList.add(mem);
#endif
    return mem;
}

ANT_ERROR Ant_HeapMemory::free(LPVOID mem)
{
    if (m_hHeap == 0) {
        ErrorLog(ANT_ERR_HEAPMEM_NO_HEAP, 0);
        return ANT_ERR_HEAPMEM_NO_HEAP;
    }

#if defined(_WIN32) || defined(_WIN32_WCE)
    BOOL ret = FALSE;

    ret = ::HeapFree(m_hHeap, 0, mem);

    if (!ret) {
        m_lastError = ::GetLastError();
        ErrorLog(ANT_ERR_HEAPMEM_ERROR_OCCURRED, m_lastError);
        return ANT_ERR_HEAPMEM_ERROR_OCCURRED;
    }
#else
    //delete it from memory list
    Ant_List<VOID>::Iterator it = m_memList.begin();
    do {
        if (mem == it.current()) {
            it.removeCurrent();
        }
    }
    while(NULL != it());

    delete[] mem;
#endif
    return ANT_ERR_NO_ERROR;
}

LPVOID Ant_HeapMemory::reAlloc(LPVOID mem, DWORD bytes)
{
    if (m_hHeap == 0) {
        ErrorLog(ANT_ERR_HEAPMEM_NO_HEAP, 0);
        return 0;
    }

    LPVOID mem2 = 0;

    //if allocSizeLimiter is true, limit the alloc size.
    //otherwise judge the physics memory size.
    if (m_allocSizeLimiter) {
        if (bytes >= ANT_HEAPMEM_MAXSIZE) {
            ErrorLog(ANT_ERR_HEAPMEM_SIZE_LIMITED, bytes);
            return 0;
        }
    }
#if defined(_WIN32) || defined(_WIN32_WCE)

    mem2 = ::HeapReAlloc(m_hHeap, 0, mem, bytes);

    if (mem2 == 0) {
        m_lastError = ::GetLastError();
        ErrorLog(ANT_ERR_HEAPMEM_ERROR_OCCURRED, m_lastError);
        return 0;
    }
#else
    mem2 = realloc(mem, bytes);
    //reset it in memory list
    Ant_List<VOID>::Iterator it = m_memList.begin();
    do {
        if (mem == it.current()) {
            it.replaceCurrent(mem2);
        }
    }
    while(NULL != it());

#endif
    return mem2;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
