/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_UTILITYLIB_H
#   include "Ant_UtilityLib.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#define CMP_DATA_SAME       0L
#define CMP_DATA_SMALL      -1L
#define CMP_DATA_BIG        1L

#define CHENGE_INS_SORT     10L
#define QSORT_STACKSIZE     32

typedef struct {
    LONG    Right;
    LONG    Left;
} sQsStack;

/*---------------------------------------------------------------------------*/
// Functions
static VOID ShellSortSub(SHORT, VOID*, VOID*, SHORT, SHORT, LONG(*)(const VOID*, const VOID*, VOID*));

VOID pbQuickWithInsSort(VOID* CmpInfo, VOID* SortAry, SHORT DataNum, SHORT DataSize, LONG (*FuncCmp)(const VOID*, const VOID*, VOID*))
{
    LONG        left;
    LONG        right;
    LONG        front;
    LONG        back;
    BYTE*       pfront;
    BYTE*       pback;
    BYTE*       data1;
    BYTE*       data2;
    BYTE        swap_data;
    SHORT       swap_cnt;
    sQsStack*   stack_ptr;
    sQsStack*   stack_last;
    sQsStack    stack_info[QSORT_STACKSIZE];

    left = 0L;
    right = (LONG)(DataNum - 1);
    memset((VOID*)stack_info, 0, sizeof(stack_info));
    stack_ptr = stack_info;
    stack_last = &stack_info[QSORT_STACKSIZE - 1];

    while (TRUE) {
        if (stack_ptr > stack_last) {

            break;
        }

        if (right - left <= CHENGE_INS_SORT) {

            if (stack_ptr <= stack_info) {
                break;
            }

            stack_ptr--;
            left = stack_ptr->Left;
            right = stack_ptr->Right;
        }

        BYTE* axis_data = ((BYTE*)SortAry) + ((left + right) / 2L) * (LONG)DataSize;

        front = left - 1L;
        back = right + 1L;
        pfront = (BYTE*)SortAry + (front * (LONG)DataSize);
        pback = (BYTE*)SortAry + (back * (LONG)DataSize);

        while (TRUE) {
            do {
                pfront += DataSize;
                front++;
            } while (FuncCmp(pfront, axis_data, CmpInfo) < CMP_DATA_SAME);

            do {
                pback -= DataSize;
                back--;
            } while (FuncCmp(pback, axis_data, CmpInfo) > CMP_DATA_SAME);

            if (front >= back) {
                break;
            }

            data1 = pfront;
            data2 = pback;

            for (swap_cnt = 0; swap_cnt < DataSize; swap_cnt++) {
                swap_data = *data1;
                *data1 = *data2;
                *data2 = swap_data;

                data1++;
                data2++;
            }

            if (axis_data ==  pfront) {
                axis_data = pback;
            }
            else if (axis_data == pback) {
                axis_data = pfront;
            }
        }

        if (front - left > right - back) {
            if (front - left > CHENGE_INS_SORT) {
                stack_ptr->Left = left;
                stack_ptr->Right = front - 1L;
                stack_ptr++;
            }

            left = back + 1L;
        }
        else {
            if (right - back > CHENGE_INS_SORT) {
                stack_ptr->Left = back + 1L;
                stack_ptr->Right = right;
                stack_ptr++;
            }

            right = front - 1L;
        }
    }

    ShellSortSub(1, CmpInfo, SortAry, DataNum, DataSize, FuncCmp);
}

VOID pbShellSort(VOID* CmpInfo, VOID* SortAry, SHORT DataNum, SHORT DataSize, LONG (*FuncCmp)(const VOID*, const VOID*, VOID*))
{
    SHORT skip;

    for (skip = 1; skip <= DataNum / 9; skip = (SHORT)((3 * skip) + 1)) ;

    while (skip > 0) {

        ShellSortSub(skip, CmpInfo, SortAry, DataNum, DataSize, FuncCmp);

        skip /= 3;
    }
}

static VOID ShellSortSub(SHORT SkipNum, VOID* CmpInfo, VOID* SortAry, SHORT DataNum, SHORT DataSize, LONG (*FuncCmp)(const VOID*, const VOID*, VOID*))
{
    BYTE*    ptrsort1;
    BYTE*    ptrsort2;
    BYTE*    ptrsort3;
    SHORT    skip_num;
    SHORT    sort_cnt1;
    SHORT    sort_cnt2;
    BYTE*    data1;
    BYTE*    data2;
    BYTE     swap_data;
    SHORT    swap_cnt;

    if (SkipNum > 1) {
        skip_num = SkipNum;
    }
    else {
        skip_num = 1;
    }

    ptrsort1 = (BYTE*)SortAry;

    for (sort_cnt1 = skip_num; sort_cnt1 < DataNum; sort_cnt1++) {

        ptrsort2 = ptrsort1;
        ptrsort3 = ptrsort2 + (skip_num * DataSize);
        ptrsort1 += DataSize;

        for (sort_cnt2 = sort_cnt1 - skip_num; sort_cnt2 >= 0; sort_cnt2 -= skip_num) {

            if (FuncCmp(ptrsort2, ptrsort3, CmpInfo) > CMP_DATA_SAME) {

                data1 = ptrsort2;
                data2 = ptrsort3;

                for (swap_cnt = 0; swap_cnt < DataSize; swap_cnt++) {
                    swap_data = *data1;
                    *data1 = *data2;
                    *data2 = swap_data;

                    data1++;
                    data2++;
                }

                ptrsort3 = ptrsort2;
                ptrsort2 -= (skip_num * DataSize);
            }
            else {
                break;
            }
        }
    }
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
