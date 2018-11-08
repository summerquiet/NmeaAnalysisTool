/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_STRINGDEF_H
#define ANT_STRINGDEF_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

// Default Charset
const INT STRING_CHARSET_DEFAULT = 0;

// The max size you could store c string in a Ant_String with codepage other than default.
const DWORD MAX_TRANSLATE_SIZE = MAX_PATH;

// name Code Page Identifiers

// Chinese Code page GB18030. Compatible with GB2312 and GBK.
const DWORD ANT_CP_GB18030 = 54936;

// Chinese Code page GBK. Compatible with GB2312.
const DWORD ANT_CP_GBK = 936;

// English Code page ANSI - Latin I for English.
const DWORD ANT_CP_LANTI = 1252;

// Japanese Code page Shift-JIS.
const DWORD ANT_CP_SJIS = 932;

// Unicode Code page UTF-8
const DWORD ANT_CP_UTF8 = 65001;

// 7-bit ASCII Code page
const DWORD ANT_CP_US_ASCII_7BIT = 20127;

// English Code page ISO-Latin I, ISO 8859-1
const DWORD ANT_CP_ISO_LANTI = 28591;

// Central Europe, ISO 8859-2
const DWORD ANT_CP_ISO_EUROPE = 28592;

// English Code page ISO-Latin 3, ISO 8859-3
const DWORD ANT_CP_ISO_LANT3 = 28593;

// ISO 8859-4 Baltic
const DWORD ANT_CP_ISO_BALTIC = 28594;

// ISO 8859-5 Cyrillic
const DWORD ANT_CP_ISO_CYRILLIC = 28595;

// ISO 8859-6 Arabic
const DWORD ANT_CP_ISO_ARABIC = 28596;

// ISO 8859-7 Greek
const DWORD ANT_CP_ISO_GREEK = 28597;

// ISO 8859-8 Hebrew
const DWORD ANT_CP_ISO_HEBREW = 28598;

// ISO 8859-9 Latin 5
const DWORD ANT_CP_ISO_LANT5 = 28599;

// ISO 8859-15 Latin 9
const DWORD ANT_CP_ISO_LANT9 = 28605;

// EUC for JP
const DWORD ANT_CP_EUC_JP = 20932;

// Mark for UTF16
const DWORD ANT_CP_UTF16 = DWORD(-1);

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_STRINGDEF_H

/*---------------------------------------------------------------------------*/
/* EOF */
