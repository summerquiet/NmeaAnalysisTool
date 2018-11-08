/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_MEMORY_H
#define ANT_MEMORY_H
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

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare
/**
 * @brief class to operate on memory block.
 *
 * @detail it represents a single memory block and defines operations on it.
 */
class ANTLIB_API Ant_Memory : public virtual Ant_Object
{
public:

    Ant_Memory();
    explicit Ant_Memory(DWORD size, BOOL clr_flg = FALSE);
    Ant_Memory(const Ant_Memory& src);
    virtual ~Ant_Memory();
    Ant_Memory& operator = (const Ant_Memory& src);

    BYTE* alloc(DWORD size);
    BYTE* calloc(DWORD size);
    VOID free();
    inline BYTE* getAddress() const {return addr;}
    inline DWORD getSize() const {return memory_size;}

    inline static DWORD getTotalSharedMemCount() {return total_sharedmem_count;}
    inline static DWORD getTotalLocalMemCount() {return total_localmem_count;}
    inline static DWORD getTotalSharedMemSize() {return total_sharedmem_size;}
    inline static DWORD getTotalLocalMemSize() {return total_localmem_size;}

protected:

    BYTE*   addr;
    DWORD   memory_size;

private:

    static volatile DWORD   total_sharedmem_count;
    static volatile DWORD   total_localmem_count;
    static volatile DWORD   total_sharedmem_size;
    static volatile DWORD   total_localmem_size;
    static Ant_SyncObj      measure_sync;

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_MEMORY_H

/*---------------------------------------------------------------------------*/
/* EOF */
