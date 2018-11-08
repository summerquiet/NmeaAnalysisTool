/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_MATHLIB_H
#    include "Ant_MathLib.h"
#endif
#include <math.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

//#define  Loop_Cnt  3

/*---------------------------------------------------------------------------*/
// Functions
WORD hypotenuse(SHORT x, SHORT y)
{
    ULONG    hy;
    ULONG    dx;
    ULONG    dy;

    dx= M_abs (x);
    dy= M_abs (y);

    if (dx < dy) {
        hy = dx;
        dx = dy;
        dy = hy;
    }

    if (dy == 0) {
       return (WORD)dx;
    }

    hy  = 100 * dy / dx;
    hy *= hy;
    hy  = hy*100000 / (40000 + hy);
    dx  = dx + 2 * dx * hy / 100000;
    dy *= hy;

    return (WORD)dx;
}

FLOAT pbHypotenuseF(FLOAT DeltaX, FLOAT DeltaY)
{
    INT      clac_cnt;
    FLOAT    tmp_calc;
    FLOAT    delta_x;
    FLOAT    delta_y;

    delta_x = (FLOAT)fabs((DOUBLE)DeltaX);
    delta_y = (FLOAT)fabs((DOUBLE)DeltaY);

    if  (delta_x < delta_y) {
        tmp_calc = delta_x;
        delta_x = delta_y;
        delta_y = tmp_calc;
    }

    if (delta_y == 0.0F) {
        return (delta_x);
    }

    for (clac_cnt = 0; clac_cnt < 2; clac_cnt++) {
        tmp_calc = delta_y / delta_x;
        tmp_calc *= tmp_calc;
        tmp_calc /= (4.0F + tmp_calc);
        delta_x += (2.0F * delta_x * tmp_calc);
        delta_y *= tmp_calc;
    }

    return (delta_x);
}

LONG lhypotenuse(LONG x, LONG y)
{
    DWORD dx = (DWORD)M_abs(x);
    DWORD dy = (DWORD)M_abs(y);

    DWORD val = (dx)*(dx) + (dy)*(dy);
    if (val > 0) {
        DWORD temp = val;
        if (dx != dy) {
            temp = val / (DWORD)M_abs((LONG)(dx - dy));
            if (temp == 0) return 0;
        }
        DWORD last = 0;
        do {
            last = temp;
            temp = (val / temp + temp) / 2;
        }
        while ((temp != 1) && (temp < last));
        return (LONG)last;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
