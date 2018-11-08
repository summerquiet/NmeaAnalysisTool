/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_AUTOSYNC_H
#define ANT_AUTOSYNC_H
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
 *  class Ant_AutoSync
 *
 * The class to synchronize automatically.
 *
 * @code
 *
 * Please use the class of Ant_AutoSync like below:
 *
 * VOID
 * XXXXClass::XXXfunction()
 * {
 *     ...
 *     Ant_AutoSync cSync(s_cSync); // s_cSync is the object of Ant_SyncObj
 *     ...
 * }
 * @endcode
 *
 */
class ANTLIB_API Ant_AutoSync : public virtual Ant_Object
{
public:

    Ant_SyncObj& m_cSync;

    /**
     * Construction.
     *
     * @param cSync : The object of class Ant_SyncObj.
     */
    Ant_AutoSync(Ant_SyncObj& cSync);

    /**
     * Destruction.
     */
    ~Ant_AutoSync();

private:

    // Disable the copy constructor and operator =
    Ant_AutoSync(const Ant_AutoSync& cSync);
    Ant_AutoSync& operator=(const Ant_AutoSync& cSync);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_AUTOSYNC_H

/*---------------------------------------------------------------------------*/
/* EOF */
