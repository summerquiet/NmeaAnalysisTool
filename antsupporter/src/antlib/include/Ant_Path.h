/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_PATH_H
#define ANT_PATH_H
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

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class delare

// external class declaration
class Ant_PathImpl;

/**
 * @class Ant_Path
 * @brief The class to process paths
 */
class ANTLIB_API Ant_Path : public virtual Ant_Object
{
public:

    /**
     * @brief Constructor
     *
     * @param[in]    strPathName    The path to be processed.
     */
    explicit Ant_Path(const Ant_String& strPathName);

    /**
     * @brief Destructor
     */
    ~Ant_Path();

    /**
     * @brief Path separator
     *
     * @return Path separator according to the native platform.
     */
    static XCHAR separator();

    /**
     * @brief Convert the path to native platform
     *
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
     */
    BOOL toNative();

    /**
     * @brief Convert the path to absolute path
     *
     * If the path is relative, this function can convert it to absolute path according to the current directory you specified.
     *
     * @param[in]    strCurrentDir    Current directory.
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
     * @remarks After this function, the path is absolute.
     */
    BOOL toAbsolute(const Ant_String& strCurrentDir);

    /**
     * @brief Parse user define path
     * *
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
     */
    BOOL parseUserDefPath();

    /**
     * @brief Check whether the path is valid
     *
     * @return TRUE indicates that the path is valid, otherwise the path is invalid.
     */
    BOOL isValid() const;

    /**
     * @brief Check whether a path is valid
     *
     * @param[in]    strPathName    The path to be checked.
     * @return TRUE indicates that the path is valid, otherwise the path is invalid.
     */
    static BOOL isValid(const Ant_String& strPathName);

    /**
     * @brief Check whether the path is a relative path
     *
     * @return TRUE indicates that the path is a relative path, otherwise the path is not a relative path.
     */
    BOOL isRelative() const;

    /**
     * @brief Check whether a path is a relative path
     *
     * @param[in]    strPathName    The path to be checked.
     * @return TRUE indicates that the path is a relative path, otherwise the path is not a relative path.
     */
    static BOOL isRelative(const Ant_String& strPathName);

    /**
     * @brief Check whether the path is a absolute path
     *
     * @return TRUE indicates that the path is a absolute path, otherwise the path is not a absolute path.
     */
    BOOL isAbsolute() const {return !isRelative();}

    /**
     * @brief Check whether a path is a absolute path
     *
     * @param[in]    strPathName    The path to be checked.
     * @return TRUE indicates that the path is a absolute path, otherwise the path is not a absolute path.
     */
    static BOOL isAbsolute(const Ant_String& strPathName) {return !isRelative(strPathName);}

    /**
     * @brief Check whether the path is a root path
     *
     * @return TRUE indicates that the path is a root path, otherwise the path is not a root path.
     */
    BOOL isRoot() const;

    /**
     * @brief Check whether a path is a root path
     *
     * @param[in]    strPathName    The path to be checked.
     * @return TRUE indicates that the path is a root path, otherwise the path is not a root path.
     */
    static BOOL isRoot(const Ant_String& strPathName);

    /**
     * @brief Get the drive name of this path
     */
    Ant_String driveName() const;

    /**
     * @brief Get upper level directory of this path
     */
    Ant_String upperDir() const;

    /**
     * @brief Get the name of the path
     */
    Ant_String pathName() const;

    //Ant_String DirName() const;

    /**
     * @brief Get the file name of the path
     */
    Ant_String fileName() const;

    /**
     * @brief Get the base name of the file name
     */
    Ant_String baseName() const;

    /**
     * @brief Get the full base name of the file name
     */
    Ant_String fullBaseName() const;

    /**
     * @brief Get the suffix of the file name
     */
    Ant_String suffix() const;

    /**
     * @brief Get the full suffix of the file name
     */
    Ant_String fullSuffix() const;

    /**
     * @brief Append a sub file name to this path
     */
    BOOL append(const Ant_String& strFileName);

protected:

    Ant_PathImpl* m_ptr;

private:

    // Disalbe the copy constructor and operator =
    Ant_Path(const Ant_Path&);
    Ant_Path& operator = (const Ant_Path&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  //ANT_PATH_H

/*---------------------------------------------------------------------------*/
/* EOF */
