/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_FILE_H
#   include "Ant_File.h"
#endif
#ifndef ANT_FILEIMPL_H
#   include "Ant_FileImpl.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_File::Ant_File(const BOOL& bRecLog)
: m_ptr(new Ant_FileImpl(bRecLog))
{

}

Ant_File::Ant_File(const Ant_String& strFileName, const BOOL& bRecLog)
: m_ptr(new Ant_FileImpl(strFileName, bRecLog))
{

}

Ant_File::Ant_File(const XCHAR* szFileName, const BOOL& bRecLog)
: m_ptr(new Ant_FileImpl(szFileName, bRecLog))
{

}

Ant_File::Ant_File(Ant_FileImpl* ptr)
: m_ptr(ptr)
{

}

Ant_File::~Ant_File()
{
    if(NULL != m_ptr) {
        delete m_ptr;
        m_ptr = NULL;
    }
}

VOID Ant_File::setFileName(const Ant_String& strFileName)
{
    if(NULL != m_ptr) {
        m_ptr->setFileName(strFileName);
    }
}

BOOL Ant_File::open(const INT& iOpenMode)
{
    return m_ptr ? m_ptr->open(iOpenMode) : FALSE;
}

BOOL Ant_File::close()
{
    return m_ptr ? m_ptr->close() : FALSE;
}

BOOL Ant_File::isOpen() const
{
    return m_ptr ? m_ptr->isOpen() : FALSE;
}

DWORD Ant_File::read(VOID* pBuff, const DWORD& dwMaxSize)
{
    return m_ptr ? m_ptr->read(pBuff, dwMaxSize) : 0;
}

DWORD Ant_File::write(const VOID* pBuff, const DWORD& dwSize)
{
    return m_ptr ? m_ptr->write(pBuff, dwSize) : 0;
}

LONG Ant_File::seek(const INT& iOffset, const Ant_FileSeekMode& eMode)
{
    return m_ptr ? m_ptr->seek(iOffset, eMode) : -1;
}

BOOL Ant_File::atEnd()
{
    return m_ptr ? m_ptr->atEnd() : FALSE;
}

BOOL Ant_File::flush()
{
    return m_ptr ? m_ptr->flush() : FALSE;
}

BOOL Ant_File::exists() const
{
    return m_ptr ? m_ptr->exists() : FALSE;
}

BOOL Ant_File::exists(const Ant_String& strFileName)
{
    Ant_File cFile(strFileName);
    return cFile.exists();
}

BOOL Ant_File::remove()
{
    return m_ptr ? m_ptr->remove() : FALSE;
}

BOOL Ant_File::remove(const Ant_String& strFileName)
{
    Ant_File cFile(strFileName);
    return cFile.remove();
}

BOOL Ant_File::moveTo(const Ant_String& strNewName)
{
    return m_ptr ? m_ptr->moveTo(strNewName) : FALSE;
}

BOOL Ant_File::move(const Ant_String& strOldName, const Ant_String& strNewName)
{
    Ant_File cFile(strOldName);
    return cFile.moveTo(strNewName);
}

BOOL Ant_File::copyTo(const Ant_String& strDstFileName)
{
    return m_ptr ? m_ptr->copyTo(strDstFileName) : FALSE;
}

BOOL Ant_File::copy(const Ant_String& strSrcFileName, const Ant_String& strDstFileName)
{
    Ant_File cFile(strSrcFileName);
    return cFile.copyTo(strDstFileName);
}

BOOL Ant_File::makeLink(const Ant_String& strLinkName)
{
    return m_ptr ? m_ptr->makeLink(strLinkName) : FALSE;
}

BOOL Ant_File::makeLink(const Ant_String& strFileName, const Ant_String& strLinkName)
{
    Ant_File cFile(strFileName);
    return cFile.makeLink(strLinkName);
}

Ant_String Ant_File::linkTarget()
{
    return m_ptr ? m_ptr->linkTarget() : Ant_String();
}

Ant_String Ant_File::linkTarget(const Ant_String& strLinkName)
{
    Ant_File cFile(strLinkName);
    return cFile.linkTarget();
}

BOOL Ant_File::setSize(const LONG& lSize)
{
    return m_ptr ? m_ptr->setSize(lSize) : FALSE;
}

BOOL Ant_File::setSize(const Ant_String& strFileName, const LONG& lSize)
{
    Ant_File cFile(strFileName);
    return cFile.setSize(lSize);
}

BOOL Ant_File::setPermissions(const DWORD& dwPerms)
{
    return m_ptr ? m_ptr->setPermissions(dwPerms) : FALSE;
}

BOOL Ant_File::setPermissions(const Ant_String& strFileName, const DWORD& dwPerms)
{
    Ant_File cFile(strFileName);
    return cFile.setPermissions(dwPerms);
}

BOOL Ant_File::setEndOfFile()
{
    return m_ptr ? m_ptr->setEndOfFile() : FALSE;
}

Ant_FileInfo Ant_File::fileInfo() const
{
    return m_ptr ? Ant_FileInfo(m_ptr->fileName()) : Ant_FileInfo();
}

Ant_FileError Ant_File::getLastError() const
{
    return m_ptr ? m_ptr->getLastError() : ANT_FER_NoError;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
