/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_MATHLIB_H
#   include "Ant_MathLib.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#define COMPILE_TBATAN2_VERSION 5

// cos table
#define COSSIN_TABLE_STEP       0x40
#define COSSIN_TABLE_RESOLUTION (DEGREE45 / COSSIN_TABLE_STEP)

// atan table
#define ATAN_TABLE_RESOLUTION   128
#define ATAN_TABLE_STEP         (0x8000 / ATAN_TABLE_RESOLUTION)

// CosSinTable
struct uCOSSIN { 
  WORD  cos;
  WORD  sin;
};

static SHORT tbatan(SHORT w); /* (-1.0=-0x8000; 0.0=0x0000; 1.0=+0x8000) */

/*---------------------------------------------------------------------------*/
// Table
static const struct uCOSSIN CosSinTable[128+1] = {
  /* cos      sin         Entry No.  (HEX)      (deg.) */
  {0x0000,  0x0000},   /*     0       0000      0.0000 */
  {0x0002,  0x0192},   /*     1       0040      0.3516 */
  {0x000a,  0x0324},   /*     2       0080      0.7031 */
  {0x0016,  0x04b6},   /*     3       00c0      1.0547 */
  {0x0027,  0x0648},   /*     4       0100      1.4062 */
  {0x003e,  0x07da},   /*     5       0140      1.7578 */
  {0x0059,  0x096c},   /*     6       0180      2.1094 */
  {0x0079,  0x0afe},   /*     7       01c0      2.4609 */
  {0x009e,  0x0c90},   /*     8       0200      2.8125 */
  {0x00c8,  0x0e21},   /*     9       0240      3.1641 */
  {0x00f7,  0x0fb3},   /*     a       0280      3.5156 */
  {0x012a,  0x1144},   /*     b       02c0      3.8672 */
  {0x0163,  0x12d5},   /*     c       0300      4.2188 */
  {0x01a1,  0x1466},   /*     d       0340      4.5703 */
  {0x01e3,  0x15f7},   /*     e       0380      4.9219 */
  {0x022b,  0x1787},   /*     f       03c0      5.2734 */
  {0x0277,  0x1918},   /*    10       0400      5.6250 */
  {0x02c8,  0x1aa8},   /*    11       0440      5.9766 */
  {0x031f,  0x1c38},   /*    12       0480      6.3281 */
  {0x037a,  0x1dc7},   /*    13       04c0      6.6797 */
  {0x03da,  0x1f56},   /*    14       0500      7.0312 */
  {0x043f,  0x20e5},   /*    15       0540      7.3828 */
  {0x04a8,  0x2274},   /*    16       0580      7.7344 */
  {0x0517,  0x2402},   /*    17       05c0      8.0859 */
  {0x058b,  0x2590},   /*    18       0600      8.4375 */
  {0x0603,  0x271e},   /*    19       0640      8.7891 */
  {0x0680,  0x28ab},   /*    1a       0680      9.1406 */
  {0x0703,  0x2a38},   /*    1b       06c0      9.4922 */
  {0x078a,  0x2bc4},   /*    1c       0700      9.8438 */
  {0x0816,  0x2d50},   /*    1d       0740     10.1953 */
  {0x08a6,  0x2edc},   /*    1e       0780     10.5469 */
  {0x093c,  0x3067},   /*    1f       07c0     10.8984 */
  {0x09d7,  0x31f1},   /*    20       0800     11.2500 */
  {0x0a76,  0x337c},   /*    21       0840     11.6016 */
  {0x0b1a,  0x3505},   /*    22       0880     11.9531 */
  {0x0bc3,  0x368e},   /*    23       08c0     12.3047 */
  {0x0c71,  0x3817},   /*    24       0900     12.6562 */
  {0x0d23,  0x399f},   /*    25       0940     13.0078 */
  {0x0ddb,  0x3b27},   /*    26       0980     13.3594 */
  {0x0e97,  0x3cae},   /*    27       09c0     13.7109 */
  {0x0f58,  0x3e34},   /*    28       0a00     14.0625 */
  {0x101e,  0x3fba},   /*    29       0a40     14.4141 */
  {0x10e8,  0x413f},   /*    2a       0a80     14.7656 */
  {0x11b8,  0x42c3},   /*    2b       0ac0     15.1172 */
  {0x128c,  0x4447},   /*    2c       0b00     15.4688 */
  {0x1365,  0x45cb},   /*    2d       0b40     15.8203 */
  {0x1442,  0x474d},   /*    2e       0b80     16.1719 */
  {0x1525,  0x48cf},   /*    2f       0bc0     16.5234 */
  {0x160c,  0x4a50},   /*    30       0c00     16.8750 */
  {0x16f8,  0x4bd1},   /*    31       0c40     17.2266 */
  {0x17e8,  0x4d50},   /*    32       0c80     17.5781 */
  {0x18de,  0x4ecf},   /*    33       0cc0     17.9297 */
  {0x19d7,  0x504d},   /*    34       0d00     18.2812 */
  {0x1ad6,  0x51cb},   /*    35       0d40     18.6328 */
  {0x1bd9,  0x5348},   /*    36       0d80     18.9844 */
  {0x1ce1,  0x54c3},   /*    37       0dc0     19.3359 */
  {0x1dee,  0x563e},   /*    38       0e00     19.6875 */
  {0x1eff,  0x57b9},   /*    39       0e40     20.0391 */
  {0x2015,  0x5932},   /*    3a       0e80     20.3906 */
  {0x2130,  0x5aaa},   /*    3b       0ec0     20.7422 */
  {0x224f,  0x5c22},   /*    3c       0f00     21.0938 */
  {0x2373,  0x5d99},   /*    3d       0f40     21.4453 */
  {0x249b,  0x5f0f},   /*    3e       0f80     21.7969 */
  {0x25c8,  0x6084},   /*    3f       0fc0     22.1484 */
  {0x26f9,  0x61f8},   /*    40       1000     22.5000 */
  {0x282f,  0x636b},   /*    41       1040     22.8516 */
  {0x296a,  0x64dd},   /*    42       1080     23.2031 */
  {0x2aa9,  0x664e},   /*    43       10c0     23.5547 */
  {0x2bed,  0x67be},   /*    44       1100     23.9062 */
  {0x2d35,  0x692d},   /*    45       1140     24.2578 */
  {0x2e82,  0x6a9b},   /*    46       1180     24.6094 */
  {0x2fd3,  0x6c08},   /*    47       11c0     24.9609 */
  {0x3128,  0x6d74},   /*    48       1200     25.3125 */
  {0x3282,  0x6edf},   /*    49       1240     25.6641 */
  {0x33e1,  0x7049},   /*    4a       1280     26.0156 */
  {0x3544,  0x71b2},   /*    4b       12c0     26.3672 */
  {0x36ab,  0x731a},   /*    4c       1300     26.7188 */
  {0x3817,  0x7480},   /*    4d       1340     27.0703 */
  {0x3987,  0x75e6},   /*    4e       1380     27.4219 */
  {0x3afc,  0x774a},   /*    4f       13c0     27.7734 */
  {0x3c75,  0x78ad},   /*    50       1400     28.1250 */
  {0x3df2,  0x7a10},   /*    51       1440     28.4766 */
  {0x3f74,  0x7b70},   /*    52       1480     28.8281 */
  {0x40fa,  0x7cd0},   /*    53       14c0     29.1797 */
  {0x4284,  0x7e2f},   /*    54       1500     29.5312 */
  {0x4413,  0x7f8c},   /*    55       1540     29.8828 */
  {0x45a5,  0x80e8},   /*    56       1580     30.2344 */
  {0x473c,  0x8243},   /*    57       15c0     30.5859 */
  {0x48d8,  0x839c},   /*    58       1600     30.9375 */
  {0x4a77,  0x84f5},   /*    59       1640     31.2891 */
  {0x4c1b,  0x864c},   /*    5a       1680     31.6406 */
  {0x4dc3,  0x87a1},   /*    5b       16c0     31.9922 */
  {0x4f6f,  0x88f6},   /*    5c       1700     32.3438 */
  {0x5120,  0x8a49},   /*    5d       1740     32.6953 */
  {0x52d4,  0x8b9a},   /*    5e       1780     33.0469 */
  {0x548d,  0x8ceb},   /*    5f       17c0     33.3984 */
  {0x564a,  0x8e3a},   /*    60       1800     33.7500 */
  {0x580a,  0x8f88},   /*    61       1840     34.1016 */
  {0x59cf,  0x90d4},   /*    62       1880     34.4531 */
  {0x5b98,  0x921f},   /*    63       18c0     34.8047 */
  {0x5d66,  0x9368},   /*    64       1900     35.1562 */
  {0x5f37,  0x94b0},   /*    65       1940     35.5078 */
  {0x610c,  0x95f7},   /*    66       1980     35.8594 */
  {0x62e5,  0x973c},   /*    67       19c0     36.2109 */
  {0x64c2,  0x9880},   /*    68       1a00     36.5625 */
  {0x66a3,  0x99c2},   /*    69       1a40     36.9141 */
  {0x6888,  0x9b03},   /*    6a       1a80     37.2656 */
  {0x6a71,  0x9c42},   /*    6b       1ac0     37.6172 */
  {0x6c5e,  0x9d80},   /*    6c       1b00     37.9688 */
  {0x6e4f,  0x9ebc},   /*    6d       1b40     38.3203 */
  {0x7043,  0x9ff7},   /*    6e       1b80     38.6719 */
  {0x723c,  0xa130},   /*    6f       1bc0     39.0234 */
  {0x7438,  0xa268},   /*    70       1c00     39.3750 */
  {0x7638,  0xa39e},   /*    71       1c40     39.7266 */
  {0x783c,  0xa4d2},   /*    72       1c80     40.0781 */
  {0x7a44,  0xa605},   /*    73       1cc0     40.4297 */
  {0x7c4f,  0xa736},   /*    74       1d00     40.7812 */
  {0x7e5e,  0xa866},   /*    75       1d40     41.1328 */
  {0x8071,  0xa994},   /*    76       1d80     41.4844 */
  {0x8288,  0xaac1},   /*    77       1dc0     41.8359 */
  {0x84a2,  0xabeb},   /*    78       1e00     42.1875 */
  {0x86c0,  0xad14},   /*    79       1e40     42.5391 */
  {0x88e2,  0xae3c},   /*    7a       1e80     42.8906 */
  {0x8b07,  0xaf62},   /*    7b       1ec0     43.2422 */
  {0x8d2f,  0xb086},   /*    7c       1f00     43.5938 */
  {0x8f5c,  0xb1a8},   /*    7d       1f40     43.9453 */
  {0x918c,  0xb2c9},   /*    7e       1f80     44.2969 */
  {0x93bf,  0xb3e8},   /*    7f       1fc0     44.6484 */
  {0x95f6,  0xb505},   /*    80       2000     45.0000 */
};

static const WORD AtanTable_V5[128] = {
  /* atan                 Entry No.    y/x      (deg.) */
  0x0000,              /*     0        0.0000   0.0000 */
  0x028c,              /*     1        0.0078   0.4476 */
  0x0518,              /*     2        0.0156   0.8952 */
  0x07a3,              /*     3        0.0234   1.3426 */
  0x0a2f,              /*     4        0.0312   1.7899 */
  0x0cba,              /*     5        0.0391   2.2370 */
  0x0f45,              /*     6        0.0469   2.6838 */
  0x11cf,              /*     7        0.0547   3.1302 */
  0x1458,              /*     8        0.0625   3.5763 */
  0x16e1,              /*     9        0.0703   4.0220 */
  0x196a,              /*     a        0.0781   4.4672 */
  0x1bf1,              /*     b        0.0859   4.9118 */
  0x1e78,              /*     c        0.0938   5.3558 */
  0x20fe,              /*     d        0.1016   5.7992 */
  0x2382,              /*     e        0.1094   6.2419 */
  0x2606,              /*     f        0.1172   6.6839 */
  0x2889,              /*    10        0.1250   7.1250 */
  0x2b0a,              /*    11        0.1328   7.5653 */
  0x2d8a,              /*    12        0.1406   8.0047 */
  0x3008,              /*    13        0.1484   8.4432 */
  0x3285,              /*    14        0.1562   8.8807 */
  0x3501,              /*    15        0.1641   9.3171 */
  0x377b,              /*    16        0.1719   9.7524 */
  0x39f3,              /*    17        0.1797  10.1866 */
  0x3c6a,              /*    18        0.1875  10.6197 */
  0x3edf,              /*    19        0.1953  11.0515 */
  0x4152,              /*    1a        0.2031  11.4820 */
  0x43c3,              /*    1b        0.2109  11.9112 */
  0x4632,              /*    1c        0.2188  12.3391 */
  0x489f,              /*    1d        0.2266  12.7656 */
  0x4b0a,              /*    1e        0.2344  13.1906 */
  0x4d73,              /*    1f        0.2422  13.6142 */
  0x4fda,              /*    20        0.2500  14.0362 */
  0x523e,              /*    21        0.2578  14.4568 */
  0x54a0,              /*    22        0.2656  14.8757 */
  0x5700,              /*    23        0.2734  15.2930 */
  0x595d,              /*    24        0.2812  15.7086 */
  0x5bb8,              /*    25        0.2891  16.1226 */
  0x5e11,              /*    26        0.2969  16.5348 */
  0x6066,              /*    27        0.3047  16.9453 */
  0x62ba,              /*    28        0.3125  17.3540 */
  0x650a,              /*    29        0.3203  17.7609 */
  0x6758,              /*    2a        0.3281  18.1660 */
  0x69a3,              /*    2b        0.3359  18.5691 */
  0x6bec,              /*    2c        0.3438  18.9704 */
  0x6e31,              /*    2d        0.3516  19.3698 */
  0x7074,              /*    2e        0.3594  19.7672 */
  0x72b4,              /*    2f        0.3672  20.1626 */
  0x74f1,              /*    30        0.3750  20.5560 */
  0x772b,              /*    31        0.3828  20.9475 */
  0x7962,              /*    32        0.3906  21.3369 */
  0x7b96,              /*    33        0.3984  21.7242 */
  0x7dc7,              /*    34        0.4062  22.1094 */
  0x7ff5,              /*    35        0.4141  22.4926 */
  0x8220,              /*    36        0.4219  22.8737 */
  0x8448,              /*    37        0.4297  23.2526 */
  0x866d,              /*    38        0.4375  23.6294 */
  0x888e,              /*    39        0.4453  24.0040 */
  0x8aad,              /*    3a        0.4531  24.3765 */
  0x8cc8,              /*    3b        0.4609  24.7467 */
  0x8ee0,              /*    3c        0.4688  25.1148 */
  0x90f5,              /*    3d        0.4766  25.4807 */
  0x9307,              /*    3e        0.4844  25.8444 */
  0x9515,              /*    3f        0.4922  26.2058 */
  0x9720,              /*    40        0.5000  26.5651 */
  0x9928,              /*    41        0.5078  26.9220 */
  0x9b2d,              /*    42        0.5156  27.2768 */
  0x9d2e,              /*    43        0.5234  27.6292 */
  0x9f2c,              /*    44        0.5312  27.9795 */
  0xa127,              /*    45        0.5391  28.3274 */
  0xa31e,              /*    46        0.5469  28.6731 */
  0xa512,              /*    47        0.5547  29.0166 */
  0xa703,              /*    48        0.5625  29.3578 */
  0xa8f1,              /*    49        0.5703  29.6967 */
  0xaadb,              /*    4a        0.5781  30.0333 */
  0xacc2,              /*    4b        0.5859  30.3676 */
  0xaea6,              /*    4c        0.5938  30.6997 */
  0xb086,              /*    4d        0.6016  31.0295 */
  0xb263,              /*    4e        0.6094  31.3571 */
  0xb43d,              /*    4f        0.6172  31.6824 */
  0xb613,              /*    50        0.6250  32.0054 */
  0xb7e6,              /*    51        0.6328  32.3261 */
  0xb9b6,              /*    52        0.6406  32.6446 */
  0xbb83,              /*    53        0.6484  32.9609 */
  0xbd4c,              /*    54        0.6562  33.2749 */
  0xbf12,              /*    55        0.6641  33.5866 */
  0xc0d5,              /*    56        0.6719  33.8962 */
  0xc294,              /*    57        0.6797  34.2035 */
  0xc451,              /*    58        0.6875  34.5085 */
  0xc60a,              /*    59        0.6953  34.8114 */
  0xc7c0,              /*    5a        0.7031  35.1120 */
  0xc972,              /*    5b        0.7109  35.4104 */
  0xcb22,              /*    5c        0.7188  35.7067 */
  0xccce,              /*    5d        0.7266  36.0007 */
  0xce77,              /*    5e        0.7344  36.2926 */
  0xd01d,              /*    5f        0.7422  36.5823 */
  0xd1c0,              /*    60        0.7500  36.8699 */
  0xd35f,              /*    61        0.7578  37.1553 */
  0xd4fc,              /*    62        0.7656  37.4386 */
  0xd695,              /*    63        0.7734  37.7197 */
  0xd82c,              /*    64        0.7812  37.9987 */
  0xd9bf,              /*    65        0.7891  38.2756 */
  0xdb4f,              /*    66        0.7969  38.5505 */
  0xdcdc,              /*    67        0.8047  38.8232 */
  0xde67,              /*    68        0.8125  39.0939 */
  0xdfee,              /*    69        0.8203  39.3625 */
  0xe172,              /*    6a        0.8281  39.6290 */
  0xe2f3,              /*    6b        0.8359  39.8935 */
  0xe471,              /*    6c        0.8438  40.1560 */
  0xe5ed,              /*    6d        0.8516  40.4165 */
  0xe765,              /*    6e        0.8594  40.6749 */
  0xe8db,              /*    6f        0.8672  40.9314 */
  0xea4d,              /*    70        0.8750  41.1859 */
  0xebbd,              /*    71        0.8828  41.4385 */
  0xed2a,              /*    72        0.8906  41.6891 */
  0xee94,              /*    73        0.8984  41.9377 */
  0xeffc,              /*    74        0.9062  42.1844 */
  0xf160,              /*    75        0.9141  42.4293 */
  0xf2c2,              /*    76        0.9219  42.6722 */
  0xf421,              /*    77        0.9297  42.9132 */
  0xf57d,              /*    78        0.9375  43.1524 */
  0xf6d7,              /*    79        0.9453  43.3897 */
  0xf82e,              /*    7a        0.9531  43.6252 */
  0xf982,              /*    7b        0.9609  43.8588 */
  0xfad4,              /*    7c        0.9688  44.0906 */
  0xfc23,              /*    7d        0.9766  44.3206 */
  0xfd6f,              /*    7e        0.9844  44.5489 */
  0xfeb9,              /*    7f        0.9922  44.7753 */
};

/*---------------------------------------------------------------------------*/
// Functions

void tbcossin(SHORT theta, COSSIN* cs)
{
    struct uCOSSIN tb;
    WORD    flag;
    INT     index;
    SHORT   surplus;
    SHORT   tmp_swap;
#ifndef NDEBUG
    SHORT   d_theta = theta;
    SHORT   d_phi;
#endif

    flag = (WORD)theta ^ ((WORD)theta>>1);

    if (theta & 0x2000) {
        theta = -theta;
    }
    theta &= 0x3fff;

#ifndef NDEBUG
    d_phi = 0;
    switch (d_theta & 0xe000) {
        case 0x0000: d_phi =  d_theta             ; break;
        case 0x2000: d_phi = DEGREE90  - d_theta  ; break;
        case 0x4000: d_phi =  d_theta  - DEGREE90 ; break;
        case 0x6000: d_phi = DEGREE180 - d_theta  ; break;
        case 0x8000: d_phi =  d_theta  + DEGREE180; break;
        case 0xa000: d_phi = -DEGREE90 - d_theta  ; break;
        case 0xc000: d_phi =  d_theta  + DEGREE90 ; break;
        case 0xe000: d_phi = -d_theta             ; break;
    }
#endif

    index   = theta / COSSIN_TABLE_STEP;
    surplus = theta % COSSIN_TABLE_STEP;

    tb = CosSinTable[index];

    if (theta <= 415)
      goto cos_end;
    tb.cos += surplus;
    if (theta <= 1248)
      goto cos_end;
    tb.cos += surplus;
    if (theta <= 2088)
      goto cos_end;
    tb.cos += surplus;
    if (theta <= 2944)
        goto cos_end;
    tb.cos += surplus;
    if (theta <= 3819)
        goto cos_end;
    tb.cos += surplus;
    if (theta <= 4725)
        goto cos_end;
    tb.cos += surplus;
    if (theta <= 5670)
        goto cos_end;
    tb.cos += surplus;
    if (theta <= 6670)
        goto cos_end;
    tb.cos += surplus;
    if (theta <= 7748)
        goto cos_end;
    tb.cos += surplus;
    cos_end:

    tb.sin += surplus*4;
    if (8057 < theta)
        goto sin_end;
    tb.sin += surplus;
    if (5263 < theta)
        goto sin_end;
    tb.sin += surplus;
    sin_end:

    tb.cos = tb.cos/4 - 0x8000;
    tb.sin = -(tb.sin/2);

    if (flag & 0x2000) {
        tmp_swap = tb.cos;
        tb.cos = tb.sin;
        tb.sin = tmp_swap;
    }
    if (0 == (flag & 0x4000)) {
        tb.cos = -tb.cos;
        if ((SHORT)tb.cos < 0) {
            tb.cos = 0x7fff;
        }
    }
    if (0 == (flag & 0x8000)) {
        tb.sin = -tb.sin;
        if ((SHORT)tb.sin < 0) {
            tb.sin = 0x7fff;
        }
    }

    *cs = *(COSSIN*)&tb;
}

SHORT tbcos(SHORT theta)
{
    COSSIN cs;

    tbcossin (theta, &cs);

    return (cs.cos);
}

SHORT tbsin(SHORT theta)
{
    COSSIN cs;

    tbcossin (theta, &cs);

    return (cs.sin);
}

static SHORT tbatan(SHORT sw)
{
    INT     sign;
    WORD    uw;
    INT     index;
    SHORT   surplus;
    WORD    tb;

    sign = 0;
    if (sw < 0) {
        if (-0x8000 == sw) {
            tb = -0x2000;
            goto end;
        }
        sw = -sw;
        sign = !0;
    }
    uw = sw;

    index = uw / ATAN_TABLE_STEP;
    surplus = uw % ATAN_TABLE_STEP;

    tb = AtanTable_V5[index];

    tb += surplus;
    if (27369 < uw)
        goto atan_end;

    tb += surplus;
    if (4467 < uw)
        goto atan_end;

    tb += surplus;

atan_end:
    tb /= 8;

    if (0 != sign)
        tb = -tb;

end:
    return (tb);
}

#if (COMPILE_TBATAN2_VERSION == 5)
SHORT tbatan2(SHORT dx, SHORT dy)
{
    SHORT numerator, denominator;
    SHORT area;
    SHORT delta;
    SHORT theta;

    if ((0 == dx) && (0 == dy)) {
        theta = 0;
        goto end;
    }
    if ((-0x8000 == dx) || (-0x8000 == dy)) {
        theta = -1;
        goto end;
    }

    if (0 < dx) {
       /* X = x, Y = y */
      if ((-dx <= dy) && (dy < dx)) {
          numerator   = dy;
          denominator = dx;
          area        = DEGREE0;
          goto brk;
      }
    }

    if (0 < dy) {
       /* X = y, Y = -x */
      if ((-dy <= -dx) && (-dx < dy)) {
          numerator   = -dx;
          denominator =  dy;
          area        = DEGREE90;
          goto brk;
      }
    }


    if (dx < 0) {
       /* X = -x, Y = -y */
      if ((dx <= -dy) && (-dy < -dx)) {
          numerator   = -dy;
          denominator = -dx;
          area        = (SHORT)DEGREE180;
          goto brk;
      }
    }


    if (dy < 0) {
       /* X = -y, Y = x */
      if ((dy <= dx) && (dx < -dy)) {
          numerator   = dx;
          denominator = -dy;
          area        = (SHORT)DEGREE270;
          goto brk;
      }
    }

    theta = -1;
    goto end;

brk:
    delta = (SHORT)((LONG)numerator * 0x8000 / denominator);

    theta = tbatan (delta);

    theta += area;

end:
    return (theta);
}
#endif  // (COMPILE_TBATAN2_VERSION == 5)

INT lnang(SHORT dx, SHORT dy, LNANG* rc)
{
    int error_code;

//    int_save_registers ();

    if ((0==dx) && (0==dy)) {
        error_code = -1;
    } 
    else {
        rc->ang = tbatan2 (dx, dy);
        tbcossin (rc->ang, (COSSIN*)&(rc->cos));
        error_code = 0;
    }

//  int_restore_registers (); 
    return (error_code);
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
