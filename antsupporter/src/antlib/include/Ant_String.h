/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_STRING_H
#define ANT_STRING_H
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
#ifndef ANT_STRINGDEF_H
#   include "Ant_StringDef.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

typedef BOOL (*Judger)(const XCHAR* chVal, USHORT& chLen);

/**
 * @brief A String manipulate class
 *
 * A class manage the buffer, codepage, serialization, deserialization and etc of the string.
 * We strongly suggest you to use this class instead of C-style string in your code.
 *
 */
class ANTLIB_API Ant_String : public virtual Ant_Object
{
public:

    /**
     * Construct an non-string. This string even does not have its buffer.
     * It is different from empty string which has a '\\0' in its own buffer.
     */
    Ant_String();

    /**
     * Copy constructor.
     */
    Ant_String(const Ant_String& rhs);

    /**
     * Construct a string with a XCHAR array.
     * If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
     * @param p The XCHAR array.
     */
    Ant_String(const XCHAR* p);

    /**
     * Construct a string with a XCHAR array and a specified length.
     * If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
     * @param p The XCHAR array.
     * @param l Length to set to Ant_String, (not include the terminator).
     */
    Ant_String(const XCHAR* p, INT l);

#ifndef ANT_STRING_UNICODE
    /**
     * Construct a string with a WCHAR array.
     * @param p The WCHAR array.
     */
    Ant_String(const WCHAR* p);

    /**
     * Construct a string with a WCHAR array and a specified length.
     * @param p The WCHAR array.
     * @param l Length to set to Ant_String, (not include the terminator).
     */
    Ant_String(const WCHAR* p, INT l);
#endif

    /**
     * Construct a string with a CHAR array and specified codepage.
     * @param dwCodepage Code page of the string. See \ref CPIden "Code Page Identifiers".
     * @param p The CHAR array.
     * note When this CHAR array is translated into UTF16, shorter than MAX_TRANSLATE_SIZE could perform a fast translate.\n
     *      Otherwise, it would be slow.
     */
    Ant_String(DWORD dwCodepage, const CHAR* p);

    /**
     * Construct a string with a CHAR array and specified codepage and length.
     * @param dwCodepage Code page of the string. See \ref CPIden "Code Page Identifiers".
     * @param p The CHAR array.
     * @param l Length to set to Ant_String, (not include the terminator).
     * note When this CHAR array is translated into UTF16, shorter than MAX_TRANSLATE_SIZE could perform a fast translate.\n
     *      Otherwise, it would be slow.
     */
    Ant_String(DWORD dwCodepage, const CHAR* p, INT l);

    /**
     * Distructor
     */
    virtual ~Ant_String();

    /**
     * Assign a Ant_String to Ant_String.
     */
    Ant_String& operator = (const Ant_String& rhs);

    /**
     * Concatenate a Ant_String at the end of this Ant_String.
     */
    Ant_String& operator += (const Ant_String& rhs);

    /**
     * Set XCHAR array into Ant_String with codepage.
     *
     * @param    p The XCHAR array. If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL set(const XCHAR* p);

    /**
     * Set XCHAR array into Ant_String with codepage and specified length.
     *
     * @param    p The XCHAR array. If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
     * @param    l The XCHAR array length, (not include the terminator).
     * @return   TRUE indicate success, vis versa.
     */
    BOOL set(const XCHAR* p, INT l);

#ifndef ANT_STRING_UNICODE
    /**
     * Set WCHAR array into Ant_String with codepage.
     *
     * @param    p The WCHAR array.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL set(const WCHAR* p);

    /**
     * Set WCHAR array into Ant_String with codepage and specified length.
     *
     * @param    p The WCHAR array.
     * @param    l The WCHAR array length, (not include the terminator).
     * @return   TRUE indicate success, vis versa.
     */
    BOOL set(const WCHAR* p, INT l);
#endif

    /**
     * Set CHAR array into Ant_String with codepage and specified length.
     *
     * @param    dwCodePage The encode of the CHAR array. See \ref CPIden "Code Page Identifiers".
     * @param    p The CHAR array.
     * @param    l The CHAR array length, (not include the terminator).
     * @return   TRUE indicate success, vis versa.
     * note When this CHAR array is translated into UTF16, shorter than MAX_TRANSLATE_SIZE could perform a fast translate.\n
     *      Otherwise, it would be slow.
     */
    BOOL set(DWORD dwCodePage, const CHAR* p, INT l);

    /**
     * \overload
     * Set CHAR array into Ant_String with codepage.
     *
     * @param    dwCodePage The encode of the CHAR array. See \ref CPIden "Code Page Identifiers".
     * @param    p the CHAR array.
     * @return   TRUE indicate success, vis versa.
     * note When this CHAR array is translated into UTF16, shorter than MAX_TRANSLATE_SIZE could perform a fast translate.\n
     *      Otherwise, it would be slow.
     */
    BOOL set(DWORD dwCodePage, const CHAR* p);

    /**
     * Set UTF16 string.
     *
     * @param    p UTF16 string
     * @return   TRUE indicate success, vis versa.
     */
    BOOL setUTF16(const UCHAR16* p);

    /**
     * Set UTF16 coded string. (Endian code according to COMP_OPT_LITTLE_ENDIAN)
     *
     * @param    p UTF16 string
     * @param    l  Length of the string, in WCHAR for Windows, or in UINT16 in Linux.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL setUTF16(const UCHAR16* p, INT l);

    /**
     * Set UTF16 Big-Endian coded string.
     *
     * @param    p UTF16 Big-Endian coded string
     * @return   TRUE indicate success, vis versa.
     */
    BOOL setUTF16BE(const UCHAR16* p);

    /**
     * Set UTF16 Big-Endian coded string.
     *
     * @param    p UTF16 Big-Endian coded string
     * @param    l  Length of the string, in WCHAR for Windows, or in UINT16 in Linux.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL setUTF16BE(const UCHAR16* p, INT l);

    /**
     * Set UTF16 Little-Endian coded string.
     *
     * @param    p UTF16 Little-Endian coded string
     * @return   TRUE indicate success, vis versa.
     */
    BOOL setUTF16LE(const UCHAR16* p);

    /**
     * Set UTF16 Little-Endian coded string.
     *
     * @param    p UTF16 Little-Endian coded string
     * @param    l  Length of the string, in WCHAR for Windows, or in UINT16 in Linux.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL setUTF16LE(const UCHAR16* p, INT l);

    /**
     * Set UTF16 coded string with a BOM symbol ahead.
     *
     * @param    p UTF16 coded string with a BOM symbol ahead
     * @return   TRUE indicate success, vis versa.
     */
    BOOL setUTF16BOM(const UCHAR16* p);

    /**
     * Set UTF16 coded string with a BOM symbol ahead.
     *
     * @param    p UTF16 coded string with a BOM symbol ahead
     * @param    l Length of the string include BOM, in WCHAR for Windows, or in UINT16 in Linux.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL setUTF16BOM(const UCHAR16* p, INT l);

    /**
     * GetString
     *
     * @param    p Buffer head to receive the XCHAR array. If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
     * @param    l Size of the buffer, (include the terminator).
     * @return   The received CHAR array length.
     */
    INT getString(XCHAR* p, INT l) const;

#ifndef ANT_STRING_UNICODE
   /**
     * GetString
     *
     * @param    p Buffer head to receive the WCHAR array.
     * @param    l Size of the buffer.
     * @return   The received CHAR array length.
     */
    INT getString(WCHAR* p, INT l) const;
#endif

    /**
     * GetString with codepage.
     *
     * @param    dwCodePage The codepage you want the CHAR array to be encoded. See \ref CPIden "Code Page Identifiers".
     * @param    p Buffer head to receive the CHAR array.
     * @param    l Size of the buffer, (include the terminator).
     * @return   The received CHAR array length.
     */
    INT getString(DWORD dwCodePage, CHAR* p, INT l) const;

    /**
     * GetStringLength with codepage.
     *
     * @param    dwCodePage The codepage you want to be encoded. See \ref CPIden "Code Page Identifiers".
     * @return   the max length to be encoded.
     */
    INT getStringLength(DWORD dwCodePage) const;

    /**
     * Get UTF16 string. (Endian code according to COMP_OPT_LITTLE_ENDIAN)
     *
     * @param    p Buffer to put the string.
     * @param    l Buffer size in WCHAR for Windows, or in UINT16 in Linux.
     * @return   If the p is NULL or l is 0, returns the buffer size to contain this string.\n
     *              Otherwise, it returns the size written to the buffer. 0 indicate failure.
     */
    INT getUTF16(UCHAR16 *p, INT l) const;

    /**
     * Get UTF16 Big_Endian coded string.
     *
     * @param    p Buffer to put the string.
     * @param    l Buffer size in WCHAR for Windows, or in UINT16 in Linux.
     * @return   If the p is NULL or l is 0, returns the buffer size to contain this string.\n
     *              Otherwise, it returns the size written to the buffer. 0 indicate failure.
     */
    INT getUTF16BE(UCHAR16 *p, INT l) const;

    /**
     * Get UTF16 Little_Endian coded string.
     *
     * @param    p Buffer to put the string.
     * @param    l Buffer size in WCHAR for Windows, or in UINT16 in Linux.
     * @return   If the p is NULL or l is 0, returns the buffer size to contain this string.\n
     *              Otherwise, it returns the size written to the buffer. 0 indicate failure.
     */
    INT getUTF16LE(UCHAR16 *p, INT l) const;

    /**
     * Get UTF16 coded string with a BOM symbol ahead.
     *
     * @param    p Buffer to put the string.
     * @param    l Buffer size in WCHAR for Windows, or in UINT16 in Linux.
     * @return   If the p is NULL or l is 0, returns the buffer size to contain this string.\n
     *              Otherwise, it returns the size written to the buffer. 0 indicate failure.
     */
    INT getUTF16BOM(UCHAR16 *p, INT l) const;

    /**
     * Add a XCHAR array to string.
     *
     * @param    p XCHAR array to append. If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL add(const XCHAR* p);

    /**
     * Add a XCHAR array to string.
     *
     * @param    p XCHAR array to append. If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
     * @param    l XCHAR length to append, (not include the terminator).
     * @return   TRUE indicate success, vis versa.
     */
    BOOL add(const XCHAR* p, INT l);

#ifndef ANT_STRING_UNICODE
    /**
     * Add a WCHAR array to string.
     *
     * @param    p WCHAR array to append.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL add(const WCHAR* p);

    /**
     * Add a WCHAR array to string.
     *
     * @param    p WCHAR array to append.
     * @param    l WCHAR length to append, (not include the terminator).
     * @return   TRUE indicate success, vis versa.
     */
    BOOL add(const WCHAR* p, INT l);
#endif

    /**
     * Append one CHAR array to string.
     *
     * @param    dwCodePage Code page of the CHAR array. See \ref CPIden "Code Page Identifiers".
     * @param    p Null-terminated CHAR array.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL add(DWORD dwCodePage, const CHAR* p);

    /**
     * Add
     *
     * @param    dwCodePage Code page of the CHAR array. See \ref CPIden "Code Page Identifiers".
     * @param    p Char array to append.
     * @param    l Char length to append, (not include the terminator).
     * @return   TRUE indicate success, vis versa.
     */
    BOOL add(DWORD dwCodePage, const CHAR* p, INT l);

    /**
     * Get Serialize Size of this Ant_String object.
     *
     * @return   Serialized size in bytes.
     */
    virtual DWORD getSerializeSize() const;

    /**
     * Serialize this object.
     *
     * @param    buffer Buffer to put the serialized object.
     * @param    size The size of the buffer.
     * @return   TRUE indicate success, vis versa.
     */
    virtual BOOL serialize(BYTE* const buffer, const DWORD size) const;

    /**
     * Deserialize a Ant_String object from buffer.
     *
     * @param   buffer The buffer which contains a serialized Ant_String Object.
     * @return   TRUE indicate success, vis versa.
     */
    virtual BOOL deserialize(const BYTE* const buffer);

    /**
     * Get String Buffer
     *
     * @return   XCHAR* the XCHAR array. If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
     */
    const XCHAR* getString() const;

    /**
     * Get String Buffer length, (not include the terminator).
     *
     * @return   INT the XCHAR array length. If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
     */
    INT getLength() const;

    /**
     * This interface is used to implicit convert Ant_String to a XCHAR array.
     */
    operator const XCHAR*() const;

    /**
     * Compare two Ant_String whether equal.
     */
    BOOL operator == (const Ant_String& lhr) const;

    /**
     * Compare two Ant_String whether not equal.
     */
    BOOL operator != (const Ant_String& lhr) const;

    /**
     * Set formatted data into Ant_String.
     *
     * @param    maxlen the formatted result's string len.
     * @param    format format string.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL format(INT maxlen, const XCHAR* format, ...);

    /**
     * Set formatted data into Ant_String.
     *
     * @param    format format string.
     * @return   TRUE indicate success, vis versa.
     */
    BOOL format(const XCHAR* format, ...);

    /**
     * Convert lower case to uppercase.
     *
     * @return   TRUE indicate success, vis versa.
     */
     BOOL upperCase();

    /**
     * Convert uppercase to lowercase.
     *
     * @return   TRUE indicate success, vis versa.
     */
     BOOL lowerCase();

    /**
     * Replace the character with \a ch on the specified position \a pos
     *
     * @param[in]    pos    The position of the character to be replaced. The first is 0.
     * @param[in]    ch    The character to be replaced to.
     *
     * @return   TRUE indicate success, vis versa.
     */
    BOOL replace(INT pos, XCHAR ch);

    /**
     * Trim the leading spaces
     *
     */
    BOOL trimLeft();

    /**
     * Trim the trailing spaces
     *
     */
    BOOL trimRight();

    /**
     * Trim the both leading and trailing spaces
     *
     */
    BOOL trim();

    /**
     * Trim the leading spaces
     *
     */
    BOOL trimLeftEx(Judger pfunc_forward);

    /**
     * Trim the trailing spaces
     *
     */
    BOOL trimRightEx(Judger pfunc_backward);

    /**
     * Trim the both leading and trailing spaces
     *
     */
    BOOL trimEx(Judger pfunc_forward, Judger pfunc_backward);

    /**
     * Check whether the string is end with the specified character \a ch
     */
    BOOL endWith(XCHAR ch) const;

    XCHAR operator[](INT offset) const;
    Ant_String left(INT len) const;
    Ant_String right(INT len) const;
    BOOL takeLeft(INT len);
    BOOL takeRight(INT len);
    Ant_String mid(INT first) const;
    Ant_String mid(INT first, INT len) const;
    BOOL takeMid(INT first);
    BOOL takeMid(INT first, INT len);

    INT replace(const XCHAR* src, const XCHAR* des);

    INT find(const XCHAR* substr) const;
    INT find(const XCHAR* substr, INT start) const;

    virtual Ant_String* clone();
    virtual VOID copy(const Ant_String* src);

protected:

    //Switch between big-endian and little-endian.
    VOID switchEndian(UCHAR16 *pBuffer, INT nLength) const;

    //Get UTF16 buffer length.
    INT utf16Strlen(const UCHAR16* p) const;

    //Release the string buffer.
    virtual VOID releaseStrBuff();
    virtual XCHAR* strAlloc(size_t);
    virtual VOID strFree(XCHAR*, size_t);
    virtual XCHAR* strRealloc(XCHAR*, size_t, size_t);
    virtual BOOL isNullBuffer() const;

private:

    XCHAR*  m_szStrBuff;    // XCHAR array. If Ant_STRING_UNICODE is not defined, it uses UTF-8 codepage.
    INT     m_nStrLen;      // equal XCHAR array length - 1

};

///This global operator is used to concatenate two Ant_String together.
ANTLIB_API const Ant_String operator + (const Ant_String& lhs, const Ant_String& rhs);

/// Compare Ant_String to XCHAR array.
ANTLIB_API BOOL operator == (const Ant_String& lhs, const XCHAR* rhs);
/// Compare XCHAR array to Ant_String.
ANTLIB_API BOOL operator == (const XCHAR* rhs, const Ant_String& lhs);
/// Compare Ant_String to XCHAR array for not equal.
ANTLIB_API BOOL operator != (const Ant_String& lhs, const XCHAR* rhs);
/// Compare XCHAR array to Ant_String for not equal.
ANTLIB_API BOOL operator != (const XCHAR* rhs, const Ant_String& lhs);

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_STRING_H

/*---------------------------------------------------------------------------*/
/* EOF */
