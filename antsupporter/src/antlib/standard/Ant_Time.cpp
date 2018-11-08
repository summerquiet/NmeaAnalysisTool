/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_TIME_H
#   include "Ant_Time.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
#if defined(ANT_HAVE_POSIX_API) && ANT_HAVE_POSIX_API

#include <time.h>
#if defined(__MACH__)
#   include <mach/mach_init.h>
#   include <mach/thread_act.h>
#   include <mach/mach_port.h>
#endif

void gettimespec(struct timespec *time,  int msec)
{
#if defined(_FOR_APPLE_)
    struct timeval now;
    gettimeofday(&now, NULL);
    time->tv_sec = now.tv_sec + msec / 1000;
    time->tv_nsec = now.tv_usec * 1000 + (msec % 1000) * 1000000;
#elif defined(_FOR_ANDROID_) || defined(_LINUX)
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    time->tv_sec = now.tv_sec + msec / 1000;
    time->tv_nsec = now.tv_nsec + (msec % 1000) * 1000000;
#endif

    // greater than one second
    if (time->tv_nsec >= 1000000000) {
        ++(time->tv_sec);
        time->tv_nsec -= 1000000000;
    }
}

void settimespec(struct timespec *time,  int msec)
{
    time->tv_sec += msec / 1000;
    time->tv_nsec += (msec % 1000) * 1000000;

    // greater than one second
    if (time->tv_nsec >= 1000000000) {
        time->tv_sec ++;
        time->tv_nsec -= 1000000000;
    }
}

int subtimespec(struct timespec *time1, struct timespec *time2)
{
    int ms = time1->tv_sec * 1000 + time1->tv_nsec / 1000000 - (time2->tv_sec * 1000 + time2->tv_nsec / 1000000);

    return ms;
}

void ANTLIB_API GetSystemTime(ANT_SYSTEMTIME *systime)
{
    struct timeval tv = { 0, 0 };
    gettimeofday(&tv, NULL);

    struct tm time;
    struct tm *ptime = NULL;
    ptime = gmtime_r(&tv.tv_sec, &time);
    if (ptime == &time) {
        systime->wYear      = ptime->tm_year + 1900;
        systime->wMonth     = ptime->tm_mon + 1; /* month of year (0 - 11) */
        systime->wDayOfWeek = ptime->tm_wday;
        systime->wDay       = ptime->tm_mday;
        systime->wHour      = ptime->tm_hour;
        systime->wMinute    = ptime->tm_min;
        systime->wSecond    = ptime->tm_sec;
        systime->wMilliseconds = tv.tv_usec / 1000;
    }
}

void ANTLIB_API GetLocalTime(ANT_SYSTEMTIME *loctime)
{
    struct timeval tv = { 0, 0 };
    gettimeofday(&tv, NULL);

    struct tm time;
    struct tm *ptime = NULL;
    ptime = localtime_r(&tv.tv_sec, &time);
    if (ptime == &time) {
        loctime->wYear      = ptime->tm_year + 1900;
        loctime->wMonth     = ptime->tm_mon + 1; /* month of year (0 - 11) */
        loctime->wDayOfWeek = ptime->tm_wday;
        loctime->wDay       = ptime->tm_mday;
        loctime->wHour      = ptime->tm_hour;
        loctime->wMinute    = ptime->tm_min;
        loctime->wSecond    = ptime->tm_sec;
        loctime->wMilliseconds = tv.tv_usec / 1000;
    }
}

DWORD ANTLIB_API GetTickCount()
{
#if defined(_FOR_APPLE_)
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
#elif defined(_FOR_ANDROID_) || defined(_LINUX)
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
#endif
}

DWORD ANTLIB_API GetThreadTime()
{
#if defined(__linux__)
    struct timespec time;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
#elif defined(__MACH__)
    thread_port_t thread = mach_thread_self();
    mach_msg_type_number_t count = THREAD_BASIC_INFO_COUNT;
    thread_basic_info_data_t info;
    int kr = thread_info(thread, THREAD_BASIC_INFO, (thread_info_t) &info, &count);
    mach_port_deallocate(mach_task_self(), thread);

    if (kr != KERN_SUCCESS) {
        return 0;
    }

    return info.user_time.seconds * 1000 + info.user_time.microseconds / 1000
            + info.system_time.seconds * 1000 + info.system_time.microseconds / 1000;
#else
    return 0;
#endif
}

#endif // ANT_HAVE_POSIX_API

/*---------------------------------------------------------------------------*/
// Value define
const int DaysToMonth365[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
const int DaysToMonth366[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
#define GETINTERNALTICKS(data) (((LONGLONG)data) & 0x3fffffffffffffffLL)
#define GETINTERNALKIND(data)  (((LONGLONG)data) & KindLocalAmbiguousDst)

Ant_TimeSpan::Ant_TimeSpan(INT d, INT h, INT mi, INT s, INT ms)
{
    m_ticks = dayTimeToTicks(d, h, mi, s, ms);
}

Ant_TimeSpan::Ant_TimeSpan(LONGLONG ticks)
: m_ticks(ticks)
{

}

Ant_TimeSpan::Ant_TimeSpan()
: m_ticks(0)
{

}

Ant_TimeSpan::Ant_TimeSpan(const Ant_TimeSpan& ts)
{
    m_ticks = ts.m_ticks;
}

Ant_TimeSpan& Ant_TimeSpan::operator =(const Ant_TimeSpan &ts)
{
    m_ticks = ts.m_ticks;
    return *this;
}

Ant_TimeSpan::~Ant_TimeSpan()
{

}

VOID Ant_TimeSpan::set(INT d, INT h, INT mi, INT s, INT ms)
{
    m_ticks = dayTimeToTicks(d, h, mi, s, ms);
}

BOOL Ant_TimeSpan::operator == (const Ant_TimeSpan &ts) const
{
    return (m_ticks == ts.m_ticks);
}

BOOL Ant_TimeSpan::operator != (const Ant_TimeSpan &ts) const
{
    return (m_ticks != ts.m_ticks);
}

BOOL Ant_TimeSpan::operator < (const Ant_TimeSpan &ts) const
{
    return (m_ticks < ts.m_ticks);
}

BOOL Ant_TimeSpan::operator <= (const Ant_TimeSpan &ts) const
{
    return (m_ticks <= ts.m_ticks);
}

BOOL Ant_TimeSpan::operator > (const Ant_TimeSpan &ts) const
{
    return (m_ticks > ts.m_ticks);
}

BOOL Ant_TimeSpan::operator >= (const Ant_TimeSpan &ts) const
{
    return (m_ticks >= ts.m_ticks);
}

Ant_TimeSpan& Ant_TimeSpan::operator += (const Ant_TimeSpan &ts)
{
    LONGLONG ticks = m_ticks + ts.m_ticks;
    if (((m_ticks >> 0x3f) == (ts.m_ticks >> 0x3f)) && ((m_ticks >> 0x3f) != (ticks >> 0x3f))) {
        //error
        return *this;
    }
    m_ticks = ticks;
    return *this;
}

Ant_TimeSpan& Ant_TimeSpan::operator -= (const Ant_TimeSpan &ts)
{
    LONGLONG ticks = m_ticks - ts.m_ticks;
    if (((m_ticks >> 0x3f) != (ts.m_ticks >> 0x3f)) && ((m_ticks >> 0x3f) != (ticks >> 0x3f))) {
        //error
        return *this;
    }
    m_ticks = ticks;
    return *this;
}

LONGLONG Ant_TimeSpan::timeToTicks(INT h, INT mi, INT s)
{
    LONGLONG num = (((LONGLONG)h * 0xe10L) + (mi * 60L)) + s;
    if ((num > 0xd6bf94d5e5LL) || (num < -922337203685LL)) {
        //error
        return 0;
    }
    return (num * TicksPerSecond);
}

LONGLONG Ant_TimeSpan::dayTimeToTicks(INT d, INT h, INT mi, INT s, INT ms)
{
    // ((((((d*3600)*24)+(h*3600))+(mi*60))+s)*1000) +ms
    LONGLONG num = (((((((LONGLONG)d * 0xe10L) * 0x18L) + (h * 0xe10L)) + (mi * 60L)) + s) * 0x3e8L) + ms;
    if ((num > 0x346dc5d638865LL) || (num < -922337203685477LL)) {
        //error
        return 0;
    }
    return (num * TicksPerMillisecond);
}

LONGLONG Ant_TimeSpan::dateToTicks(INT y, INT m, INT d)
{
    INT* numArray = (INT*)(isLeapYear(y) ? DaysToMonth366 : DaysToMonth365);
    INT num = y - 1;
    if (m<0 || m>12) return 0;//error
    INT num2 = ((((((num * DaysPerYear) + (num / 4)) - (num / 100)) + (num / 400)) + numArray[m - 1]) + d) - 1;
    return (num2 * TicksPerDay);
}

ULONGLONG Ant_TimeSpan::antTimeToTicks(const Ant_TimeStruct& time)
{
    LONGLONG num = dateToTicks(time.wYear, time.wMonth, time.wDay) + timeToTicks(time.wHour, time.wMinute, time.wSecond);
    num += time.wMilliseconds * TicksPerMillisecond;
    return (ULONGLONG)num;
}

Ant_TimeStruct Ant_TimeSpan::ticksToAntTime(const ULONGLONG& dateData)
{
    INT num2 = (INT) (GETINTERNALTICKS(dateData) / TicksPerDay);
    INT num3 = num2 / 0x23ab1;
    num2 -= num3 * 0x23ab1;
    INT num4 = num2 / 0x8eac;
    if (num4 == 4) {
        num4 = 3;
    }
    num2 -= num4 * 0x8eac;
    INT num5 = num2 / 0x5b5;
    num2 -= num5 * 0x5b5;
    INT num6 = num2 / 0x16d;
    if (num6 == 4) {
        num6 = 3;
    }
    INT Year = ((((num3 * 400) + (num4 * 100)) + (num5 * 4)) + num6) + 1;

    num2 -= num6 * 0x16d;
    //INT dayofyear = num2 + 1;

    INT* numArray = (INT*)(((num6 == 3) && ((num5 != 0x18) || (num4 == 3))) ? DaysToMonth366 : DaysToMonth365);
    INT index = num2 >> 6;
    while (num2 >= numArray[index]) {
        index++;
    }
    INT Month = index;
    INT Day = (num2 - numArray[index - 1]) + 1;
    INT DayWeek = dayOfWeek(Year, Month, Day);

    INT Hour = (INT)((GETINTERNALTICKS(dateData) / 0x861c46800LL) % 0x18L);
    INT Minute = (INT)((GETINTERNALTICKS(dateData) / 0x23c34600L) % 60L);
    INT Second = (INT)((GETINTERNALTICKS(dateData) / TicksPerSecond) % 60L);
    INT Milliseconds = (INT)((GETINTERNALTICKS(dateData) / TicksPerMillisecond) % 0x3e8L);

    Ant_TimeStruct time = {
        static_cast<WORD>(Year & 0xFFFF),
        static_cast<WORD>(Month & 0xFFFF),
        static_cast<WORD>(DayWeek & 0xFFFF),
        static_cast<WORD>(Day & 0xFFFF),
        static_cast<WORD>(Hour & 0xFFFF),
        static_cast<WORD>(Minute & 0xFFFF),
        static_cast<WORD>(Second & 0xFFFF),
        static_cast<WORD>(Milliseconds & 0xFFFF),
    };
    return time;
}

ULONGLONG Ant_TimeSpan::add(const Ant_TimeStruct& time, const Ant_TimeSpan& ts)
{
    ULONGLONG dateData = antTimeToTicks(time);
    LONGLONG internalTicks = GETINTERNALTICKS(dateData);
    LONGLONG spanTicks = ts.m_ticks;
    if ((spanTicks < -internalTicks) || ((MaxTicks - internalTicks) < spanTicks)) {
        return 0;
    }
    return ((ULONGLONG)(internalTicks + spanTicks)) | GETINTERNALKIND(dateData);
}

ULONGLONG Ant_TimeSpan::subtract(const Ant_TimeStruct& time, const Ant_TimeSpan& ts)
{
    ULONGLONG dateData = antTimeToTicks(time);
    LONGLONG internalTicks = GETINTERNALTICKS(dateData);
    LONGLONG spanTicks = ts.m_ticks;
    if ((internalTicks < spanTicks) || (internalTicks - MaxTicks > spanTicks)) {
        return 0;
    }
    return ((ULONGLONG)(internalTicks - spanTicks)) | GETINTERNALKIND(dateData);
}

LONGLONG Ant_TimeSpan::subtract(const Ant_TimeStruct& time1, const Ant_TimeStruct& time2)
{
    ULONGLONG dateData1 = antTimeToTicks(time1);
    ULONGLONG dateData2 = antTimeToTicks(time2);

    LONGLONG internalTicks = 0;
    if (dateData1 >= dateData2) {
        internalTicks = GETINTERNALTICKS(dateData1 - dateData2);
    }
    else {
        internalTicks = (-1) * GETINTERNALTICKS(dateData2 - dateData1);
    }
    return internalTicks;
}

BOOL Ant_TimeSpan::isLeapYear(INT y)
{
    if ((y % 4) != 0){
        return FALSE;
    }
    if ((y % 100) == 0) {
        return ((y % 400) == 0) ? TRUE : FALSE;
    }
    return TRUE;
}

INT Ant_TimeSpan::dayOfWeek(INT y, INT m, INT d)
{
    if(m <= 2) {
        m += 12;
        y --;
    }
    return (y + (y / 4) - (y / 100) + (y / 400) + ((13 * m + 8) / 5) + d) % 7;
}

Ant_TimeSpan operator + (const Ant_TimeSpan& lhs, const Ant_TimeSpan& rhs)
{
    Ant_TimeSpan ts(lhs);
    ts += rhs;
    return ts;
}

Ant_TimeSpan operator - (const Ant_TimeSpan& lhs, const Ant_TimeSpan& rhs)
{
    Ant_TimeSpan ts(lhs);
    ts -= rhs;
    return(ts);
}

Ant_Time::Ant_Time()
{
    memset(&m_cTime, 0, sizeof(Ant_TimeStruct));
}

Ant_Time::Ant_Time(INT y, INT m, INT d, INT h, INT mi, INT s, INT ms)
{
    set(y,m,d,h,mi,s,ms);
}

Ant_Time::Ant_Time(const Ant_Time& t)
{
    memcpy(&m_cTime, &t.m_cTime, sizeof(Ant_TimeStruct));
}

Ant_Time& Ant_Time::operator = (const Ant_Time& t)
{
    if (this == &t) {
        return *this;
    }
    memcpy(&m_cTime, &t.m_cTime, sizeof(Ant_TimeStruct));
    return *this;
}

Ant_Time::~Ant_Time()
{

}

VOID Ant_Time::getNow()
{
    ANT_SYSTEMTIME time;
    GetLocalTime(&time);
    set(time);
}

VOID Ant_Time::getUTCNow()
{
    ANT_SYSTEMTIME time;
    GetSystemTime(&time);
    set(time);
}

DWORD Ant_Time::getTickCount()
{
    return GetTickCount();
}

VOID Ant_Time::set(ANT_SYSTEMTIME& time)
{
    m_cTime.wYear           = time.wYear;
    m_cTime.wMonth          = time.wMonth;
    m_cTime.wDayOfWeek      = time.wDayOfWeek;
    m_cTime.wDay            = time.wDay;
    m_cTime.wHour           = time.wHour;
    m_cTime.wMinute         = time.wMinute;
    m_cTime.wSecond         = time.wSecond;
    m_cTime.wMilliseconds   = time.wMilliseconds;
}

VOID Ant_Time::set(INT y, INT m, INT d, INT h, INT mi, INT s, INT ms)
{
    setDate(y,m,d);
    setTime(h,mi,s,ms);
}

VOID Ant_Time::setTime(INT h, INT mi, INT s, INT ms)
{
    m_cTime.wHour           = h & 0xFFFF;
    m_cTime.wMinute         = mi & 0xFFFF;
    m_cTime.wSecond         = s & 0xFFFF;
    m_cTime.wMilliseconds   = ms & 0xFFFF;
}

VOID Ant_Time::setDate(INT y, INT m, INT d)
{
    m_cTime.wYear       = y & 0xFFFF;
    m_cTime.wMonth      = m & 0xFFFF;
    m_cTime.wDay        = d & 0xFFFF;
    m_cTime.wDayOfWeek  = Ant_TimeSpan::dayOfWeek(y, m, d) & 0xFFFF;
}

BOOL Ant_Time::operator == (const Ant_Time& time) const
{
    return (0 == memcmp(&m_cTime, &time.m_cTime, sizeof(Ant_TimeStruct)));
}

BOOL Ant_Time::operator != (const Ant_Time& time) const
{
    return (0 != memcmp(&m_cTime, &time.m_cTime, sizeof(Ant_TimeStruct)));
}

Ant_Time& Ant_Time::operator += (const Ant_TimeSpan& ts)
{
    ULONGLONG dateData = Ant_TimeSpan::add(m_cTime, ts);
    m_cTime = Ant_TimeSpan::ticksToAntTime(dateData);
    return *this;
}

Ant_Time& Ant_Time::operator -= (const Ant_TimeSpan& ts)
{
    ULONGLONG dateData = Ant_TimeSpan::subtract(m_cTime, ts);
    m_cTime = Ant_TimeSpan::ticksToAntTime(dateData);
    return *this;
}

BOOL Ant_Time::operator > (const Ant_Time& time) const
{
    ULONGLONG dateData = Ant_TimeSpan::antTimeToTicks(time.m_cTime);
    ULONGLONG mine = Ant_TimeSpan::antTimeToTicks(m_cTime);
    return (mine > dateData);
}

BOOL Ant_Time::operator >= (const Ant_Time& time) const
{
    ULONGLONG dateData = Ant_TimeSpan::antTimeToTicks(time.m_cTime);
    ULONGLONG mine = Ant_TimeSpan::antTimeToTicks(m_cTime);
    return (mine >= dateData);
}

BOOL Ant_Time::operator < (const Ant_Time& time) const
{
    ULONGLONG dateData = Ant_TimeSpan::antTimeToTicks(time.m_cTime);
    ULONGLONG mine = Ant_TimeSpan::antTimeToTicks(m_cTime);
    return (mine < dateData);
}

BOOL Ant_Time::operator <= (const Ant_Time& time) const
{
    ULONGLONG dateData = Ant_TimeSpan::antTimeToTicks(time.m_cTime);
    ULONGLONG mine = Ant_TimeSpan::antTimeToTicks(m_cTime);
    return (mine <= dateData);
}

BOOL Ant_Time::isValidTime() const
{
    if (m_cTime.wHour > 23 || m_cTime.wMinute > 59 || m_cTime.wSecond > 59 ) {
        return FALSE;
    }
    return TRUE;
}

BOOL Ant_Time::isValidDate() const
{
    if (m_cTime.wMonth == 0 || m_cTime.wMonth > 12) {
        return FALSE;
    }
    WORD MAX_DAY_OF_MONTH = 0;
    if (Ant_TimeSpan::isLeapYear(m_cTime.wYear)) {
        static const WORD DAY_OF_MONTH[]={0,31,29,31,30,31,30,31,31,30,31,30,31};
        MAX_DAY_OF_MONTH = DAY_OF_MONTH[m_cTime.wMonth];
    }
    else {
        static const WORD DAY_OF_MONTH[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
        MAX_DAY_OF_MONTH = DAY_OF_MONTH[m_cTime.wMonth];
    }
    if (m_cTime.wDay == 0 || m_cTime.wDay > MAX_DAY_OF_MONTH) {
        return FALSE;
    }
    return TRUE;
}

BOOL Ant_Time::isValidTimeDate() const
{
    if (isValidDate() && isValidTime()) {
        return TRUE;
    }
    return FALSE;
}

Ant_Time operator + (const Ant_Time& lhs, const Ant_TimeSpan& rhs)
{
    Ant_Time time(lhs);
    time += rhs;
    return time;
}

Ant_Time operator - (const Ant_Time& lhs, const Ant_TimeSpan& rhs)
{
    Ant_Time time(lhs);
    time -= rhs;
    return time;
}

Ant_TimeSpan operator - (const Ant_Time& lhs, const Ant_Time& rhs)
{
    LONGLONG ticks = Ant_TimeSpan::subtract(lhs.m_cTime, rhs.m_cTime);
    return Ant_TimeSpan(ticks);
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
