/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_ABSTRACTDATABLOCK_H
#define ANT_ABSTRACTDATABLOCK_H
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
 * Definition of abstract interface for a data block.
 *
 * It only define interfaces, it does not provide implementation, you should
 * sub-class it to implement the interface.
 */
class Ant_AbstractDataBlock : public virtual Ant_Object
{
public:

    Ant_AbstractDataBlock();

    virtual ~Ant_AbstractDataBlock();

    /**
     * Get size of the data block.
     *
     * @retval size of the data block in bytes.
     */
    virtual size_t getLength() = 0;

    /**
     * Get pointer to raw data of the data block.
     *
     * @retval pointer to the data of the data block.
     */
    virtual const void* getData() = 0;

private:

    // Disable the copy constructor and operator =
    Ant_AbstractDataBlock(const Ant_AbstractDataBlock&);
    Ant_AbstractDataBlock& operator = (const Ant_AbstractDataBlock&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_ABSTRACTDATABLOCK_H

/*---------------------------------------------------------------------------*/
/* EOF */
