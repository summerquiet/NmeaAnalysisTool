/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_MEMORY_H
#   include "Ant_Memory.h"
#endif
#include <stdlib.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#define NMEM_SHARED_MEM_THR     (5*1024)

volatile DWORD  Ant_Memory::total_sharedmem_count = 0;
volatile DWORD  Ant_Memory::total_localmem_count = 0;
volatile DWORD  Ant_Memory::total_sharedmem_size = 0;
volatile DWORD  Ant_Memory::total_localmem_size = 0;
Ant_SyncObj     Ant_Memory::measure_sync;

/*---------------------------------------------------------------------------*/
// Functiosn
Ant_Memory::Ant_Memory()
{
    addr = NULL;
    memory_size = 0;
}

Ant_Memory::Ant_Memory(DWORD size, BOOL clr_flg)
{
    addr   = NULL;
    memory_size = 0;
     if (clr_flg == TRUE) {
        calloc(size);
    } 
    else {
        alloc(size);
    }
}

Ant_Memory::Ant_Memory(const Ant_Memory& src)
{
    addr   = NULL;
    memory_size = 0;

    if (src.getSize() > 0) {
         alloc(src.getSize());
        memcpy(addr, src.getAddress(), src.getSize());
    }
}

Ant_Memory::~Ant_Memory()
{
    free();
}

Ant_Memory& Ant_Memory::operator = (const Ant_Memory& src)
{
    if (this == &src) return *this;

    if (src.getSize() > 0) {
        alloc(src.getSize());
        memcpy(addr, src.getAddress(), src.getSize());
    }
    else {
        free();
    }
    return *this;
}

BYTE* Ant_Memory::alloc(DWORD size)
{
    if (addr != NULL) {
        free();
    }

    addr = new BYTE[size];

    if (addr != NULL) {
        memory_size = size;
        measure_sync.syncStart();
        total_localmem_count++;
        total_localmem_size += memory_size;
        measure_sync.syncEnd();
    }

    return addr;
}

BYTE* Ant_Memory::calloc(DWORD size)
{
    if (alloc(size) != NULL){
        memset(addr, 0, memory_size);
    }
    return addr;
}

VOID Ant_Memory::free()
{
    if (addr != NULL) {
        measure_sync.syncStart();
        total_localmem_count--;
        total_localmem_size -= memory_size;
        measure_sync.syncEnd();

        delete[] addr;
        addr = NULL;
    }
    memory_size = 0;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
