/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#ifndef CXX_NMEADECODEOBJECTLIST_H
#define CXX_NMEADECODEOBJECTLIST_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
//  Include files

/*---------------------------------------------------------------------------*/
// Class define

class CNmeaDecodeObject;

/**
 * CNmeaDecodeObjectList class
 *
 * NMEA decode process object list to install a packet data
 */
class CNmeaDecodeObjectList
{
public:

    /**
     * Constructor
     *
     * @param   NONE
     *
     * @return  NONE
     */
    CNmeaDecodeObjectList();

    /**
     * Destructor
     *
     * @param   NONE
     *
     * @return  NONE
     */
    virtual ~CNmeaDecodeObjectList();

    /**
     * Push the object to the end of the list
     *
     * @param   pcObject    [IN]: need to push object
     *
     * @return  BOOL        : push action successful or not
     */
    BOOL Push(CNmeaDecodeObject* pcObject);

    /**
     * Pop the object of the head of the list 
     *
     * @param   rpcObject   [OUT]: need to pop object
     *
     * @return  BOOL        : pop action successful or not
     */
    BOOL Pop(CNmeaDecodeObject* &rpcObject);

    /**
     * Check the list empty or not
     *
     * @param   NONE
     *
     * @return  BOOL : the list is empty or not
     */
    BOOL IsEmpty(VOID);

private:

    // All members
    static const INT    OBJECT_LIST_LENGTH = 10;

    CNmeaDecodeObject*  m_pcaObjectList[OBJECT_LIST_LENGTH];
    INT                 m_iListHead;
    INT                 m_iListTail;
    BOOL                m_bIsFull;

private:

    /**
     * Check the list full or not
     *
     * @param   NONE
     *
     * @return  BOOL    : the list is full or not
     */
    BOOL IsFull(VOID);

};

/*---------------------------------------------------------------------------*/
#endif  // CXX_NMEADECODEOBJECTLIST_H

/*---------------------------------------------------------------------------*/
/* EOF */
