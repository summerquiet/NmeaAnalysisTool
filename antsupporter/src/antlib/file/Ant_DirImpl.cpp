/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_DIRIMPL_H
#   include "Ant_DirImpl.h"
#endif
#ifndef ANT_PATH_H
#   include "Ant_Path.h"
#endif
#ifndef ANT_ABSTRACTFILEENGINE_H
#   include "Ant_AbstractFileEngine.h"
#endif
#ifndef ANT_DIR_H
#   include "Ant_Dir.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_DirImpl::Ant_DirImpl(const BOOL& bRecLog)
: m_strDirPath()
, m_eLastError(ANT_FER_NoError)
, m_bRecLog(bRecLog)
{

}

Ant_DirImpl::Ant_DirImpl(const Ant_String& strDirPath, const BOOL& bRecLog)
: m_strDirPath()
, m_eLastError(ANT_FER_NoError)
, m_bRecLog(bRecLog)
{
    Ant_Path cPath(strDirPath);
    cPath.parseUserDefPath();
    m_strDirPath = cPath.pathName();
}

Ant_DirImpl::~Ant_DirImpl()
{

}

VOID Ant_DirImpl::setWorkDir(const Ant_String& strDirPath)
{
    Ant_Path cPath(strDirPath);
    cPath.parseUserDefPath();
    m_strDirPath = cPath.pathName();
}

BOOL Ant_DirImpl::exists(const Ant_String& strfileName)
{
    cleanLastError();

    Ant_Path cPath(strfileName);
    if (cPath.isRelative() && (0 != m_strDirPath.getLength())) {
        if (!cPath.toAbsolute(m_strDirPath)) {
            setLastError(ANT_FER_InvalidParam);
            return FALSE;
        }
    }

    Ant_AbstractFileEngine* pFileEngine = Ant_AbstractFileEngine::create(cPath.pathName(), m_bRecLog);
    if (NULL == pFileEngine) {
        return FALSE;
    }

    BOOL bRet = pFileEngine->exists();
    if (!bRet) {
        setLastError(pFileEngine->getLastError());
    }
    delete pFileEngine;

    return bRet;
}

BOOL Ant_DirImpl::move(const Ant_String& strOldName, const Ant_String& strNewName)
{
    cleanLastError();

    Ant_Path cOldPath(strOldName);
    if (cOldPath.isRelative() && (0 != m_strDirPath.getLength())) {
        if (!cOldPath.toAbsolute(m_strDirPath)) {
            setLastError(ANT_FER_InvalidParam);
            return FALSE;
        }
    }

    Ant_Path cNewPath(strNewName);
    if (cNewPath.isRelative() && (0 != m_strDirPath.getLength())) {
        if (!cNewPath.toAbsolute(m_strDirPath)) {
            setLastError(ANT_FER_InvalidParam);
            return FALSE;
        }
    }

    Ant_FileInfo cOldInfo(cOldPath.pathName());
    Ant_FileInfo cNewInfo(cNewPath.pathName());

    //src file not exists
    if(!cOldInfo.exists()) {
        setLastError(ANT_FER_NotExists);
        return FALSE;
    }

    //dest exist, and not the same type as src
    if((cNewInfo.exists())
        && (cNewInfo.getFileType() != cOldInfo.getFileType())) {
        setLastError(ANT_FER_AlreadyExists);
        return FALSE;
    }

    if(cNewPath.pathName() == cOldPath.pathName()) {
        return TRUE;
    }

    Ant_AbstractFileEngine* pOldFileEngine = NULL;
    pOldFileEngine = Ant_AbstractFileEngine::create(cOldPath.pathName(), m_bRecLog);
    if(NULL == pOldFileEngine) {
        return FALSE;
    }

    if(pOldFileEngine->exists()) {
        //do call rename for a same partition, file move
        BOOL bRet = pOldFileEngine->rename(cNewPath.pathName());
        if(bRet) {
            //success do return
            delete pOldFileEngine;
            return bRet;
        }
        else {
            printf("failed to rename %s \n", cNewPath.pathName().getString());
            setLastError(pOldFileEngine->getLastError());
            //leave it to the last execute
        }
    }
    else {
		setLastError(ANT_FER_NotExists);
        delete pOldFileEngine;
		return FALSE;
	}

    DWORD dwPerms = pOldFileEngine->fileFlags(ANT_FFG_PermsMask);

    //make sure if src folder could be deleted after all
    if(!cOldInfo.isWritable()) {
        //src file wasn't able to be removed
        setLastError(ANT_FER_NoAccess);
        delete pOldFileEngine;
        return FALSE;
    }
    else {
        //do nothing
    }

    Ant_AbstractFileEngine* pNewFileEngine = NULL;
    pNewFileEngine = Ant_AbstractFileEngine::create(cNewPath.upperDir(), m_bRecLog);
    if (NULL == pNewFileEngine) {
        delete pOldFileEngine;
        return FALSE;
    }

    // create the new directory
    BOOL bNewDirExist = FALSE;
    while (!pNewFileEngine->makeDir(cNewPath.fileName())) {
        if (ANT_FER_AlreadyExists == pNewFileEngine->getLastError()) {
            Ant_FileInfo cNewInfo(cNewPath.pathName());
            if (cNewInfo.isDir()) {
                bNewDirExist = TRUE;
                break;
            }
        }
        setLastError(pNewFileEngine->getLastError());
        delete pNewFileEngine;
        delete pOldFileEngine;
        return FALSE;
    }

    // copy all sub entries to the new directory
    Ant_AbstractFileEngine::Iterator* pOldDirIter = NULL;
    pOldDirIter = pOldFileEngine->entryIterator();
    if (NULL == pOldDirIter) {
        delete pNewFileEngine;
        delete pOldFileEngine;
        return FALSE;
    }
    BOOL bRet = TRUE;
    for (; !pOldDirIter->end(); ++(*pOldDirIter)) {
        if (XTEXT(".") == pOldDirIter->currentName()
            || XTEXT("..") == pOldDirIter->currentName()) {
            continue;
        }

        Ant_Path cOldPath(strOldName);
        Ant_Path cNewPath(strNewName);
        // make path
        if (!cOldPath.append(pOldDirIter->currentName())
            || !cNewPath.append(pOldDirIter->currentName())) {
            bRet = FALSE;
            break;
        }

        if (!move(cOldPath.pathName(), cNewPath.pathName())) {
            bRet = FALSE;
            break;
        }
    }
    delete pOldDirIter;

    // remove the old directory
    if (bRet && !pOldFileEngine->remove(TRUE)) {
        setLastError(pOldFileEngine->getLastError());
        bRet = FALSE;
    }

    if (bRet && !bNewDirExist) {
        // if the new directory does not exist originally, the set its permission the same as the old one.
        delete pNewFileEngine;
        pNewFileEngine = Ant_AbstractFileEngine::create(cNewPath.pathName(), m_bRecLog);
        if (NULL == pNewFileEngine)     {
            delete pOldFileEngine;
            return FALSE;
        }
        if (!pNewFileEngine->setPermissions(dwPerms)) {
            setLastError(pNewFileEngine->getLastError());
            bRet = FALSE;
        }
    }

    delete pNewFileEngine;
    delete pOldFileEngine;
    return bRet;
}

BOOL Ant_DirImpl::copy(const Ant_String& strSrcName, const Ant_String& strDstName)
{
    cleanLastError();

    Ant_Path cSrcPath(strSrcName);
    if (cSrcPath.isRelative() && (0 != m_strDirPath.getLength())) {
        if (!cSrcPath.toAbsolute(m_strDirPath)) {
            setLastError(ANT_FER_InvalidParam);
            return FALSE;
        }
    }

    Ant_Path cDstPath(strDstName);
    if (cDstPath.isRelative() && (0 != m_strDirPath.getLength())) {
        if (!cDstPath.toAbsolute(m_strDirPath)) {
            setLastError(ANT_FER_InvalidParam);
            return FALSE;
        }
    }

    if (cDstPath.pathName() == cSrcPath.pathName()) {
        return FALSE;
    }

    Ant_FileInfo cSrcInfo(cSrcPath.pathName());
    Ant_AbstractFileEngine* pSrcFileEngine = NULL;
    pSrcFileEngine = Ant_AbstractFileEngine::create(cSrcPath.pathName(), m_bRecLog);
    if (NULL == pSrcFileEngine) {
        return FALSE;
    }
    if (!cSrcInfo.isDir()) {    // the source path is not a directory, just copy it to the destination path
        BOOL bRet = pSrcFileEngine->copyTo(cDstPath.pathName());
        if (!bRet)     {
            Ant_AbstractFileEngine* pDstFileEngine = NULL;
            pDstFileEngine = Ant_AbstractFileEngine::create(cDstPath.pathName(), m_bRecLog);
            if (NULL == pDstFileEngine)         {
                return FALSE;
            }

            setLastError(pSrcFileEngine->getLastError());
            delete pDstFileEngine;
        }
        delete pSrcFileEngine;
        return bRet;
    }

    DWORD dwPerms = pSrcFileEngine->fileFlags(ANT_FFG_PermsMask);

    // the source path is a directory,
    Ant_AbstractFileEngine* pDstFileEngine = NULL;
    pDstFileEngine = Ant_AbstractFileEngine::create(cDstPath.upperDir(), m_bRecLog);
    if (NULL == pDstFileEngine) {
        delete pSrcFileEngine;
        return FALSE;
    }

    // create the destination directory
    BOOL bDstDirExist = FALSE;
    while( !pDstFileEngine->makeDir(cDstPath.fileName()) )
    {
        if (ANT_FER_AlreadyExists == pDstFileEngine->getLastError())     {
            Ant_FileInfo cNewInfo(cDstPath.pathName());
            if (cNewInfo.isDir())         {
                bDstDirExist = TRUE;
                break;
            }
        }
        setLastError(pDstFileEngine->getLastError());
        delete pDstFileEngine;
        delete pSrcFileEngine;
        return FALSE;
    }

    Ant_AbstractFileEngine::Iterator* pSrcDirIter = NULL;
    pSrcDirIter = pSrcFileEngine->entryIterator();
    if (NULL == pSrcDirIter) {
        delete pDstFileEngine;
        delete pSrcFileEngine;
        return FALSE;
    }

    BOOL bRet = TRUE;
    for (; !pSrcDirIter->end(); ++(*pSrcDirIter))
    {
        if (XTEXT(".") == pSrcDirIter->currentName()
            || XTEXT("..") == pSrcDirIter->currentName()) {
            continue;
        }

        Ant_Path cSrcPath(strSrcName);
        Ant_Path cDstPath(strDstName);
        if (!cSrcPath.append(pSrcDirIter->currentName())
            || !cDstPath.append(pSrcDirIter->currentName())) {
            bRet = FALSE;
            break;
        }

        if (!copy(cSrcPath.pathName(), cDstPath.pathName())) {
            bRet = FALSE;
            break;
        }
    }
    delete pSrcDirIter;

    if (bRet && !bDstDirExist) {
        delete pDstFileEngine;
        pDstFileEngine = Ant_AbstractFileEngine::create(cDstPath.pathName(), m_bRecLog);
        if (NULL == pDstFileEngine) {
            delete pSrcFileEngine;
            return FALSE;
        }
        if (!pDstFileEngine->setPermissions(dwPerms)) {
            setLastError(pDstFileEngine->getLastError());
            bRet = FALSE;
        }
    }

    delete pDstFileEngine;
    delete pSrcFileEngine;
    return bRet;
}

BOOL Ant_DirImpl::remove(const Ant_String& strfileName, const BOOL& bForce)
{
    cleanLastError();

    Ant_Path cPath(strfileName);
    if (cPath.isRelative() && (0 != m_strDirPath.getLength())) {
        if (!cPath.toAbsolute(m_strDirPath))     {
            setLastError(ANT_FER_InvalidParam);
            return FALSE;
        }
    }

    Ant_FileInfo cInfo(cPath.pathName());
    Ant_AbstractFileEngine* pFileEngine = NULL;
    pFileEngine = Ant_AbstractFileEngine::create(cPath.pathName(), m_bRecLog);
    if (NULL == pFileEngine) {
        return FALSE;
    }
    if (!cInfo.isDir()) {    // the source path is not a directory, remove it
        BOOL bRet = pFileEngine->remove(bForce);
        if (!bRet)     {
            setLastError(pFileEngine->getLastError());
        }
        delete pFileEngine;
        return bRet;
    }

    Ant_AbstractFileEngine::Iterator* pDirIter = NULL;
    pDirIter = pFileEngine->entryIterator();
    if (NULL == pDirIter) {
        delete pFileEngine;
        return FALSE;
    }

    // remove all sub entries in the directory
    BOOL bRet = TRUE;
    for(; !pDirIter->end(); ++(*pDirIter))
    {
        if (XTEXT(".") == pDirIter->currentName()
            || XTEXT("..") == pDirIter->currentName()) {
            continue;
        }

        Ant_Path cPath(strfileName);
        if (!cPath.append(pDirIter->currentName())) {
            bRet = FALSE;
            break;
        }
        if (!remove(cPath.pathName(), bForce)) {
            bRet = FALSE;
            break;
        }
    }

    delete pDirIter;
    pDirIter = NULL;

    if (!bRet) {
        delete pFileEngine;
        return FALSE;
    }

    // remove the directory itself
    bRet = pFileEngine->remove(bForce);
    if (!bRet) {
        setLastError(pFileEngine->getLastError());
    }
    delete pFileEngine;

    return bRet;
}

BOOL Ant_DirImpl::makeDir(const Ant_String& strDirName)
{
    cleanLastError();

    Ant_Path cPath(strDirName);
    if (cPath.isRelative() && (0 != m_strDirPath.getLength())) {
        if (!cPath.toAbsolute(m_strDirPath)) {
            setLastError(ANT_FER_InvalidParam);
            return FALSE;
        }
    }

    Ant_AbstractFileEngine* pFileEngine = Ant_AbstractFileEngine::create(cPath.upperDir(), m_bRecLog);
    if (NULL == pFileEngine) {
        return FALSE;
    }

    if (!pFileEngine->makeDir(cPath.fileName())) {
        Ant_FileInfo cInfo(cPath.pathName());
        if (!cInfo.isDir())     {
            setLastError(pFileEngine->getLastError());
            return FALSE;
        }
    }
    delete pFileEngine;

    return TRUE;
}

BOOL Ant_DirImpl::makePath(const Ant_String& strpathName)
{
    cleanLastError();

    Ant_Path cPath(strpathName);
    if (cPath.isRelative() && (0 != m_strDirPath.getLength())) {
        if (!cPath.toAbsolute(m_strDirPath)) {
            setLastError(ANT_FER_InvalidParam);
            return FALSE;
        }
    }
    Ant_String strUpper = cPath.upperDir();

    // assume the root directory exists
    if (strUpper == XTEXT("") || Ant_Path::isRoot(strUpper)) {
        return TRUE;
    }

    if (!makeDir(cPath.pathName())) {
        if (ANT_FER_NotExists == m_eLastError) {
            if (!makePath(strUpper)) {
                return FALSE;
            }

            return makeDir(cPath.pathName());
        }
        return FALSE;
    }

    return TRUE;
}


DWORD Ant_DirImpl::entryNum(const DWORD& dwFilters)
{
    Ant_AbstractFileEngine* pFileEngine = Ant_AbstractFileEngine::create(m_strDirPath, m_bRecLog);
    if (NULL == pFileEngine) {
        return 0;
    }

    Ant_AbstractFileEngine::Iterator* pDirIter = NULL;
    pDirIter = pFileEngine->entryIterator();
    if (NULL == pDirIter) {
        delete pFileEngine;
        return 0;
    }

    DWORD dwRet = 0;
    for(; !pDirIter->end(); ++(*pDirIter)) {
        if (XTEXT(".") == pDirIter->currentName()
            || XTEXT("..") == pDirIter->currentName()) {
            continue;
        }

        // TODO: check entry filters

        dwRet++;
    }

    delete pDirIter;
    delete pFileEngine;

    return dwRet;
}

Ant_DirIterator* Ant_DirImpl::entryIterator(const DWORD& dwFilters)
{
    return new Ant_DirIterator(m_strDirPath, dwFilters, m_bRecLog);
}

Ant_DirIteratorImpl::Ant_DirIteratorImpl(const Ant_String& strDirPath, const DWORD& dwFilters, const BOOL& bRecLog)
: m_strDirPath(strDirPath)
, m_cEntryList()
, m_iCur(0)
, m_bRecLog(bRecLog)
{
    initEntryList(dwFilters);
}

Ant_DirIteratorImpl::~Ant_DirIteratorImpl()
{
    for (INT i = 0; i < m_cEntryList.length(); i++)
    {
        delete m_cEntryList[i];
    }
    m_cEntryList.clear();
}

Ant_String Ant_DirIteratorImpl::currentFileName()
{
    if (m_iCur >= m_cEntryList.length()) {
        return XTEXT("");
    }
    return (*m_cEntryList[m_iCur]);
}

Ant_FileInfo Ant_DirIteratorImpl::currentFileInfo()
{
    if (m_iCur >= m_cEntryList.length()) {
        return Ant_FileInfo();
    }
    Ant_Path cPath(m_strDirPath);
    cPath.append(*m_cEntryList[m_iCur]);
    return Ant_FileInfo(cPath.pathName());
}

BOOL Ant_DirIteratorImpl::end() const
{
    return (m_iCur == m_cEntryList.length()) ? TRUE : FALSE;
}

Ant_DirIteratorImpl& Ant_DirIteratorImpl::operator ++ ()
{
    if (m_iCur < m_cEntryList.length()) {
        m_iCur++;
    }
    return *this;
}

VOID Ant_DirIteratorImpl::reset()
{
    m_iCur = 0;
}

BOOL Ant_DirIteratorImpl::initEntryList(const DWORD& dwFilters)
{
    Ant_AbstractFileEngine* pFileEngine = Ant_AbstractFileEngine::create(m_strDirPath, m_bRecLog);
    if (NULL == pFileEngine) {
        return FALSE;
    }

    Ant_AbstractFileEngine::Iterator* pDirIter = NULL;
    pDirIter = pFileEngine->entryIterator();
    if (NULL == pDirIter) {
        delete pFileEngine;
        return FALSE;
    }

    for(; !pDirIter->end(); ++(*pDirIter)) {
        if (dwFilters & ANT_FFT_NoDotAndDotDot) {
            if (XTEXT(".") == pDirIter->currentName()
                || XTEXT("..") == pDirIter->currentName()) {
                continue;
            }
        }
        if (!match(pDirIter->currentFileFlag(), dwFilters)) {
            continue;
        }
        m_cEntryList.add(new Ant_String(pDirIter->currentName()));
    }

    delete pDirIter;
    delete pFileEngine;

    return TRUE;
}

BOOL Ant_DirIteratorImpl::match(const DWORD& dwFileFlag, const DWORD& dwFilters)
{
    if (((dwFilters & ANT_FFT_NoSystem) && !(dwFileFlag & ANT_FFG_AttrSystem))
        || ((dwFilters & ANT_FFT_NoHidden) && !(dwFileFlag & ANT_FFG_AttrHidden))
        || ((dwFilters & ANT_FFT_Readable) && (dwFileFlag & ANT_FFG_PermOwnerRead))
        || ((dwFilters & ANT_FFT_Writable) && (dwFileFlag & ANT_FFG_PermOwnerWrite))
        || ((dwFilters & ANT_FFT_Executable) && (dwFileFlag & ANT_FFG_PermOwnerExe))
        || ((dwFilters & ANT_FFT_Dirs) && (dwFileFlag & ANT_FFG_TypeDirectory))
        || ((dwFilters & ANT_FFT_Files) && (dwFileFlag & ANT_FFG_TypeFile))
        || ((dwFilters & ANT_FFT_NoSymLinks) && !(dwFileFlag & ANT_FFG_TypeLink))) {

        return TRUE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
