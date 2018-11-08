/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_PATHIMPL_H
#define ANT_PATHIMPL_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_STRING_H
#   include "Ant_String.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declaration

/**
 * @class Ant_PathImpl
 * @brief The class to process paths
 */
class Ant_PathImpl
{
public:

    explicit Ant_PathImpl(const Ant_String& strPathName);
    ~Ant_PathImpl() {}

    BOOL toNative();
    BOOL toAbsolute(const Ant_String& strCurrentDir);
    BOOL parseUserDefPath();

    BOOL isValid() const {return isValid(m_strPathName);}
    BOOL isRelative() const {return isRelative(m_strPathName);}
    BOOL isRoot() const {return isRoot(m_strPathName);}

    Ant_String driveName() const;
    Ant_String upperDir() const {return upperDir(m_strPathName);}
    Ant_String pathName() const {return m_strPathName;}
    Ant_String fileName() const;
    Ant_String baseName() const;
    Ant_String fullBaseName() const;
    Ant_String suffix() const;
    Ant_String fullSuffix() const;

    BOOL append(const Ant_String& strPathName);

protected:

    BOOL toNative(Ant_String& strPathName);

    BOOL isValid(const Ant_String& strPathName) const;
    BOOL isRelative(const Ant_String& strPathName) const;
    BOOL isRoot(const Ant_String& strPathName) const;

    /**
     * if the path has upper level directory, it returns the upper level directory with the last path separator
     */
    Ant_String upperDir(const Ant_String& strPathName) const;
    Ant_String fileName(const Ant_String& strPathName) const;
    Ant_String baseName(const Ant_String& strPathName) const;
    Ant_String fullBaseName(const Ant_String& strPathName) const;
    Ant_String suffix(const Ant_String& strPathName) const;
    Ant_String fullSuffix(const Ant_String& strPathName) const;

protected:

    Ant_String  m_strPathName;
    BOOL     m_bIsNative;

private:
    
    // Disable the copy constructor and operator =
    Ant_PathImpl(const Ant_PathImpl&);
    Ant_PathImpl& operator = (const Ant_PathImpl&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_PATHIMPL_H

/*---------------------------------------------------------------------------*/
/* EOF */
