/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_MATHLIB_H
#define ANT_MATHLIB_H
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

/*---------------------------------------------------------------------------*/
// Value define

#define M_abs(ex)   ((ex)>=0 ? (ex) : -(ex))

// (360 degree = 0x10000)
# define DEGREE0    (0x0000)    /*   0 */
# define DEGREE20   (0x0e39)    /*  20 */
# define DEGREE45   (0x2000)    /*  45 */
# define DEGREE90   (0x4000)    /*  90 */
# define DEGREE135  (0x6000)    /* 135 */
# define DEGREE180  (0x8000)    /* 180 */
# define DEGREE270  (0xc000)    /* 270 */
# define DEGREE360  (0x10000)   /* 360 */
# define DEGREE450  (DEGREE360 + DEGREE90)  /*  450 */
# define DEGREE540  (DEGREE360 + DEGREE180) /*  540 */
# define DEGREE630  (DEGREE360 + DEGREE270) /*  630 */
# define DEGREE720  (DEGREE360 + DEGREE360) /*  720 */
# define DEGREE810  (DEGREE720 + DEGREE90)  /*  810 */
# define DEGREE900  (DEGREE720 + DEGREE180) /*  900 */
# define DEGREE990  (DEGREE720 + DEGREE270) /*  990 */
# define DEGREE1080 (DEGREE720 + DEGREE360) /* 1080 */

typedef struct{
  SHORT cos; /* cos * 8000H (1.0=0x7fff; -1.0=-0x8000) */
  SHORT sin; /* sin * 8000H (1.0=0x7fff; -1.0=-0x8000) */
  SHORT ang; /* (360 =10000H) */
}LNANG;

typedef struct{
  SHORT cos; /* cos ()*0x8000 */
  SHORT sin; /* sin ()*0x8000 */
}COSSIN;

/*---------------------------------------------------------------------------*/
// Functions

/*---------------------------------------------------------------------------*/
// Ant_Hypo.cpp
ANTLIB_API WORD hypotenuse(SHORT dx, SHORT dy);
ANTLIB_API FLOAT pbHypotenuseF(FLOAT DeltaX, FLOAT DeltaY);
ANTLIB_API LONG lhypotenuse(LONG b, LONG c);

/*---------------------------------------------------------------------------*/
// Ant_Trigono.cpp
ANTLIB_API void tbcossin(SHORT theta, COSSIN*);
ANTLIB_API SHORT tbcos(SHORT theta);
ANTLIB_API SHORT tbsin(SHORT theta);
ANTLIB_API SHORT tbatan2(SHORT dx, SHORT dy);
ANTLIB_API INT lnang(SHORT dlt_x, SHORT   dlt_y, LNANG* rc);

/*---------------------------------------------------------------------------*/
// Ant_stTrigono.cpp
ANTLIB_API LONG st_sin(ULONG angle);
ANTLIB_API LONG st_cos(ULONG angle);
ANTLIB_API LONG st_tan(ULONG angle);

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_MATHLIB_H

/*---------------------------------------------------------------------------*/
/* EOF */
