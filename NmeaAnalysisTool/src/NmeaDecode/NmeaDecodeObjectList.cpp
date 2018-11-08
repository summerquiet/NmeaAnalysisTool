/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
//  Include files

#ifndef CXX_NMEADECODEOBJECTLIST_H
#   include "NmeaDecodeObjectList.h"
#endif
#ifndef CXX_NMEADECODEOBJECT_H
#    include "NmeaDecodeObject.h"
#endif

/*---------------------------------------------------------------------------*/
// Constructor
CNmeaDecodeObjectList::CNmeaDecodeObjectList()
: m_iListHead(0)
, m_iListTail(0)
, m_bIsFull(FALSE)
{
    // Initialize all members
    for (INT iCnt = 0; iCnt < OBJECT_LIST_LENGTH; iCnt++) {
        m_pcaObjectList[iCnt] = NULL;
    }
}

/*---------------------------------------------------------------------------*/
// Destructor
CNmeaDecodeObjectList::~CNmeaDecodeObjectList()
{
    // Clear the list
    for (INT iCnt = 0; iCnt < OBJECT_LIST_LENGTH; iCnt++) {
        m_pcaObjectList[iCnt] = NULL;
    }

    m_iListHead = 0;
    m_iListTail = 0;
    m_bIsFull = FALSE;
}

/*---------------------------------------------------------------------------*/
// Push the object to the end of the list
BOOL CNmeaDecodeObjectList::Push(CNmeaDecodeObject* pcObject)
{
    if (IsFull()) {
        // The list is FULL
        // Can not push a object
        return FALSE;
    }

    // Move the tail position
    m_pcaObjectList[m_iListTail] = pcObject;
    m_iListTail = (m_iListTail + 1) % OBJECT_LIST_LENGTH;

    // FULL
    if (m_iListTail == m_iListHead) {
        m_bIsFull = TRUE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*/
// Pop the object of the head of the list 
BOOL CNmeaDecodeObjectList::Pop(CNmeaDecodeObject* &rpcObject)
{
    if (IsEmpty()) {
        // The list is EMPTY
        // Can not pop a object
        return FALSE;
    }

    rpcObject = m_pcaObjectList[m_iListHead];
    m_pcaObjectList[m_iListHead] = NULL;
    m_iListHead = (m_iListHead + 1) % OBJECT_LIST_LENGTH;

    // Not FULL
    if (m_bIsFull) {
        m_bIsFull = FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*/
// Check the list empty or not
BOOL CNmeaDecodeObjectList::IsEmpty(VOID)
{
    // Check the head and tail pointer
    if (m_iListHead == m_iListTail
        && !m_bIsFull) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

/*---------------------------------------------------------------------------*/
// Check the list full or not
BOOL CNmeaDecodeObjectList::IsFull(VOID)
{
    // Check next tail position and head
    if (m_iListHead == m_iListTail
        && m_bIsFull) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

/*---------------------------------------------------------------------------*/
/* EOF */
