/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_ABSTRACTFILEENGINE_H
#   include "Ant_AbstractFileEngine.h"
#endif
#ifndef ANT_NORMALFILEENGINE_H
#   include "Ant_NormalFileEngine.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_AbstractFileEngine* Ant_AbstractFileEngine::create(const Ant_String& strFileName, const BOOL& bRecLog)
{
    return new Ant_NormalFileEngine(strFileName, bRecLog);
}

BOOL Ant_AbstractFileEngine::isSequential() const
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::caseSensitive() const
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::open(INT iOpenMode)
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::close()
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::isOpen()
{
    return FALSE;
}

DWORD Ant_AbstractFileEngine::read(VOID* pBuf, const DWORD& dwMaxSize)
{
    return 0;
}

DWORD Ant_AbstractFileEngine::write(const VOID* pBuf, const DWORD& dwSize)
{
    return 0;
}

LONG Ant_AbstractFileEngine::seek(const INT& iOffset, const Ant_FileSeekMode& eMode)
{
    return -1L;
}

BOOL Ant_AbstractFileEngine::flush()
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::atEnd()
{
    return FALSE;
}

LONGLONG Ant_AbstractFileEngine::size()
{
    return -1LL;
}

BOOL Ant_AbstractFileEngine::setSize(const LONG& lSize)
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::exists()
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::remove(BOOL bForse)
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::copyTo(const Ant_String& strDstName)
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::moveTo(const Ant_String& strNewName)
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::rename(const Ant_String& strNewName)
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::makeLink(const Ant_String& strLinkName)
{
    return FALSE;
}

Ant_String Ant_AbstractFileEngine::linkTarget()
{
    return XTEXT("");
}

BOOL Ant_AbstractFileEngine::makeDir(const Ant_String& strDirName)
{
    return FALSE;
}

Ant_AbstractFileEngine::Iterator* Ant_AbstractFileEngine::entryIterator()
{
    return NULL;
}

DWORD Ant_AbstractFileEngine::fileFlags(const DWORD& dwMask)
{
    return 0;
}

BOOL Ant_AbstractFileEngine::setPermissions(const DWORD& dwPerms)
{
    return FALSE;
}

BOOL Ant_AbstractFileEngine::setEndOfFile()
{
    return FALSE;
}

Ant_String Ant_AbstractFileEngine::ownerName(const Ant_FileUser& eKind) const
{
    return Ant_String();
}

DWORD Ant_AbstractFileEngine::ownerID(const Ant_FileUser& eKind) const
{
    return 0;
}

Ant_Time Ant_AbstractFileEngine::fileTime(const Ant_FileTime& eKind)
{
    return Ant_Time();
}

Ant_FileError Ant_AbstractFileEngine::getLastError() const
{
    return ANT_FER_NoError;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
