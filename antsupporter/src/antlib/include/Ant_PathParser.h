/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_PATHPARSER_H
#define ANT_PATHPARSER_H
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
// Class declare

//The i/f to translate between user-defined path and abs path
struct ANTLIB_API Ant_PathParserIF : public virtual Ant_Object
{
    virtual ~Ant_PathParserIF() {}

    virtual BOOL parseUserDefPath(const Ant_String& strUDPath, Ant_String& strAbsPath) = 0;

    virtual BOOL parseAbsPath(const Ant_String& strAbsPath, Ant_String& strUDPath) = 0;
};

struct ANTLIB_API Ant_PathFactoryIF : public virtual Ant_Object
{
    virtual ~Ant_PathFactoryIF() {}

    virtual Ant_PathParserIF* createPathParser() = 0;
};

/**
 * @brief The class to between user-defined path and abs path
 *
 * @detail it convert virtual path to abs path.
 */
class ANTLIB_API Ant_PathParser : public Ant_PathParserIF
{
public:

    /**
     * @brief Constructor
     */
    Ant_PathParser();

    /**
     * @brief Destructor
     */
    virtual ~Ant_PathParser();

    virtual BOOL parseUserDefPath(const Ant_String& strUDPath, Ant_String& strAbsPath);

    virtual BOOL parseAbsPath(const Ant_String& strAbsPath, Ant_String& strUDPath);

protected:

    //translate from User-Defined to Abs path
    //need override it!!!
    virtual BOOL translateUD(Ant_String& strPartition);

    //get the partition name from path
    virtual BOOL getPartion(const Ant_String& strPath, Ant_String& strPartition, Ant_String& strOthers);

private:

    // Disable the copy constructor and operator =
    Ant_PathParser(const Ant_PathParser&);
    Ant_PathParser& operator=(const Ant_PathParser&);

};

extern ANTLIB_API VOID Ant_SetPathFactory(Ant_PathFactoryIF* p);
extern ANTLIB_API VOID Ant_GetPathFactory(Ant_PathFactoryIF** pp);

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_PATHPARSER_H

/*---------------------------------------------------------------------------*/
/* EOF */
