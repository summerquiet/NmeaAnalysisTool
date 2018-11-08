#ifndef __WINSELF_H__
#define __WINSELF_H__

#undef STRICT					
#define _CRT_SECURE_NO_WARNINGS	

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#undef _TCHAR_DEFINED

#define numofchar(s)		(sizeof(s) / sizeof(TCHAR))

#define isstrhead(s, h)		(_tcsncmp(s, h, _tcslen(h)) == 0)

#define isstrihead(s, h)	(_tcsncicmp(s, h, _tcslen(h)) == 0)

typedef double DOUBLE;

#ifdef __cplusplus

inline int atoi(char *s)
{
	return static_cast<int>(atol(s));
}

inline int abs(unsigned long ul)
{
	return static_cast<int>(fabs(static_cast<float>(ul)));
}

inline float sqrt(int x)
{
	return static_cast<float>(sqrtf(static_cast<float>(x)));
}

inline float atan2(int x, int y)
{
	return static_cast<float>(atan2f(static_cast<float>(x), static_cast<float>(y)));
}

inline int pow(int x, int y)
{
	return static_cast<int>(powf(static_cast<float>(x), static_cast<float>(y)));
}

inline int pow(int x, double y)
{
	return static_cast<int>(pow(static_cast<double>(x), y));
}

#endif	// __cplusplus

#include "wmx_excption.h"

#endif	// __WINSELF_H__
