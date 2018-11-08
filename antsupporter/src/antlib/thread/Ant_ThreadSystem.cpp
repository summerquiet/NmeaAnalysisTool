/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_THREADSYSTEM_H
#   include "Ant_ThreadSystem.h"
#endif

#include <cassert>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_ThreadSystem::Ant_ThreadSystem()
:m_threadTableList(NULL)
{
    m_threadTableList = new Ant_List<ANT_THREAD_TABLE>();
    if (NULL == m_threadTableList) {
        return;
    }
}

Ant_ThreadSystem::~Ant_ThreadSystem()
{
    if (NULL == m_threadTableList) {
        return;
    }
    while(m_threadTableList->hasData()) {
        //release nodes
        ANT_THREAD_TABLE* t = m_threadTableList->pop();
        if (t != NULL){
            if (t->thread_name != NULL) {
                delete[]t->thread_name;
                t->thread_name=NULL;
            }
            delete t;
            t=NULL;
        }
    }

    delete m_threadTableList;
    m_threadTableList = NULL;
}

INT Ant_ThreadSystem::registerThread(XCHAR* name, HANDLE handle, DWORD thread_id)
{
    /*
    TCHAR tstr[96];
    _sntprintf(tstr, 96, TEXT(">>>>>%s:\t\t PROC:%08X ThreadID:%08x\n"), name, GetCurrentProcessId(), thread_id);
    tstr[95] = TEXT('\0');
    OutputDebugString(tstr);

    CreateThreadInfoAccess();
    return info_access->SetThreadInfo(::GetCurrentProcessId(), (DWORD)handle, thread_id, name);
    */
    return 0;
}

VOID Ant_ThreadSystem::unregisterThread(INT thread_no)
{
    /*
    CreateThreadInfoAccess();
    info_access->ClearThreadInfo(thread_no);
    */
}

BOOL Ant_ThreadSystem::addThreadTable(const ANT_THREAD_TABLE* table)
{
    if (table == NULL) {
        return FALSE;
    }
    INT table_no = 0;
    while(1) {

        if (table[table_no].thread_name[0] == XTEXT('\0')) {
            break;
        }
        ANT_THREAD_TABLE* cpy = new ANT_THREAD_TABLE;
        if(NULL == cpy) {
            return FALSE;
        }
        size_t s = xcslen(table[table_no].thread_name);
        cpy->thread_name = new XCHAR[s + 1];
        if (NULL == cpy->thread_name) {
            return FALSE;
        }
        xcscpy(cpy->thread_name, table[table_no].thread_name);

        for (INT i = 0; i < 3; i++) {
            cpy->priority[i] = table[table_no].priority[i];
        }

        cpy->sanity_interval = table[table_no].sanity_interval;
        m_threadTableList->push(cpy);

        table_no++;
    }

    return TRUE;
}

BOOL Ant_ThreadSystem::getThreadTableInfo(const XCHAR* name, DWORD* priority, DWORD& sanity)
{
    if (NULL==name) {
        return FALSE;
    }
    assert(priority!=NULL);

    if (NULL == m_threadTableList) {
        return FALSE;
    }

    Ant_List<ANT_THREAD_TABLE>::Iterator itr=m_threadTableList->begin();
    do {
        ANT_THREAD_TABLE* cur=itr.current();
        if(NULL == cur) {
            return FALSE;
        }

        if (xcscmp(name, cur->thread_name) == 0) {
            for (INT i = 0; i < 3; i++) {
                //priority[i] = local_thread_table[table_no].priority[i];
                priority[i] = cur->priority[i];
            }
            sanity = cur->sanity_interval;
            return TRUE;
        }
    } while(NULL!=itr());

    return FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
