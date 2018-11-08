/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_PATHPARSER_H
#   include "Ant_PathParser.h"
#endif
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

static Ant_PathFactoryIF* s_pcPathFactory = NULL;

/*---------------------------------------------------------------------------*/
// Functions
VOID Ant_SetPathFactory(Ant_PathFactoryIF* p)
{
    s_pcPathFactory = p;
}

VOID Ant_GetPathFactory(Ant_PathFactoryIF** pp)
{
    if (pp != NULL) {
        *pp = s_pcPathFactory;
    }
}

Ant_PathParser::Ant_PathParser()
{

}

Ant_PathParser::~Ant_PathParser()
{

}

BOOL Ant_PathParser::parseUserDefPath(const Ant_String& strUDPath, Ant_String& strAbsPath)
{
    Ant_String strPartition;
    Ant_String strOtherPath;
    if (!getPartion(strUDPath, strPartition, strOtherPath)) {
        strAbsPath = strUDPath; //cann't parse, copy directly
        return FALSE;
    }
    ANT_PRINTF("[Ant_PathParser]getPartion [%s]-[%s]", strPartition.getString(), strOtherPath.getString());

    if (!translateUD(strPartition)) {
        strAbsPath = strUDPath; //cann't parse, copy directly
        return FALSE;
    }
    ANT_PRINTF("[Ant_PathParser]translateUD [%s]-[%s]", strPartition.getString(), strOtherPath.getString());

    strAbsPath = strPartition + strOtherPath;
    ANT_PRINTF("[Ant_PathParser]absPath [%s]", strAbsPath.getString());
    return TRUE;
}

BOOL Ant_PathParser::parseAbsPath(const Ant_String& strAbsPath, Ant_String& strUDPath)
{
    //TODO: not implement
    strUDPath = strAbsPath;
    return FALSE;
}

BOOL Ant_PathParser::translateUD(Ant_String& strPartition)
{
    //need override it.
#if !defined(_WIN32) && !defined(_WIN32_WCE)
    if (strPartition == Ant_String(XTEXT("root"))) {
        strPartition.set(XTEXT(""));
        return FALSE;
    }
#endif
    return FALSE;
}

BOOL Ant_PathParser::getPartion(const Ant_String& strPath, Ant_String& strPartition, Ant_String& strOthers)
{
    const XCHAR* pszPath = strPath.getString();
    if (pszPath == NULL) return FALSE;

    const XCHAR* pcur = pszPath;
    if (*pcur == '\\' || *pcur == '/') {
#if defined(_WIN32) || defined(_WIN32_WCE)
        return FALSE;
#else
        strPartition.set(XTEXT("root"));
        strOthers = strPath;
        return TRUE;
#endif
    }
    while (*pcur != '\0') {
        if (*pcur == '\\' || *pcur == '/') {
            break;
        }
        pcur++;
    }
    UINT nLen = pcur - pszPath;
    strPartition.set(pszPath, nLen);
    strOthers.set(pcur, strPath.getLength() - nLen);
    return TRUE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
