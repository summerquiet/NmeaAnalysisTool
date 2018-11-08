/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_TIME_H
#define ANT_TIME_H
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
#ifndef ANT_TIMEDEF_H
#   include "Ant_TimeDef.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declaration

/**
 *  class Ant_TimeSpan
 *
 * The class to calculate time.
 *
 */
class ANTLIB_API Ant_TimeSpan : public virtual Ant_Object
{
public:

    static const LONGLONG   TicksPerMillisecond     = 0x2710LL;
    static const LONGLONG   TicksPerSecond          = 0x989680LL;
    static const LONGLONG   TicksPerMinute          = 0x23c34600LL;
    static const LONGLONG   TicksPerHour            = 0x861c46800LL;
    static const LONGLONG   TicksPerDay             = 0xc92a69c000LL;
    static const LONGLONG   DaysPerYear             = 0x16d;                // 0x16d = 365
    static const LONGLONG   MinTicks                = 0x0L;
    static const LONGLONG   MaxTicks                = 0x2bca2875f4373fffLL;
    static const ULONGLONG  KindLocalAmbiguousDst   = 0xC000000000000000LL;

public:

    /**
     * Constructor
     *
     * @param d The day of the month. The valid values for this member are -24854 through 24854.
     * @param h The hour. The valid values for this member are -23 through 23.
     * @param mi The minute. The valid values for this member are -59 through 59.
     * @param s The second. The valid values for this member are -59 through 59.
     * @param ms The millisecond. The valid values for this member are -999 through 999.
     */
    Ant_TimeSpan(INT d, INT h, INT mi, INT s, INT ms = 0);

    /**
     * Copy constructor
     */
    Ant_TimeSpan(const Ant_TimeSpan& ts);

    /**
     * operator =
     */
    Ant_TimeSpan& operator = (const Ant_TimeSpan& ts);

    /**
     * Destructor
     */
    ~Ant_TimeSpan();

    /**
     * Set DateTime
     *
     * @param d The day of the month. The valid values for this member are -24854 through 24854.
     * @param h The hour. The valid values for this member are -23 through 23.
     * @param mi The minute. The valid values for this member are -59 through 59.
     * @param s The second. The valid values for this member are -59 through 59.
     * @param ms The millisecond. The valid values for this member are -999 through 999.
     */
    VOID set(INT d, INT h, INT mi, INT s, INT ms = 0);

    /**
     * Get the year which storaged in this class.
     *
     * @return INT year (-24854 ~ 24854)
     */
    INT detDay() const {return (INT)(m_ticks / Ant_TimeSpan::TicksPerDay);}

    /**
     * Get the hour which storaged in this class.
     *
     * @return INT hour (-23 ~ 23)
     */
    INT getHour() const {return (INT)((m_ticks / Ant_TimeSpan::TicksPerHour) % 0x18L);}

    /**
     * Get the minute which storaged in this class.
     *
     * @return INT minute (-59 ~ 59)
     */
    INT getMinute() const {return (INT)((m_ticks / Ant_TimeSpan::TicksPerMinute) % 60L);}

    /**
     * Get the second which storaged in this class.
     *
     * @return INT second (-59 ~ 59)
     */
    INT getSecond() const {return (INT)((m_ticks / Ant_TimeSpan::TicksPerSecond) % 60L);}

    /**
     * Get the millisecond which storaged in this class.
     *
     * @return INT millisecond (-999 ~ 999)
     */
    INT getMillisecond() const {return (INT)((m_ticks / Ant_TimeSpan::TicksPerMillisecond) % 0x3e8L);}

    /**
     * operator ==
     */
    BOOL operator == (const Ant_TimeSpan& ts) const;

    /**
     * operator !=
     */
    BOOL operator != (const Ant_TimeSpan& ts) const;

    /**
     * operator <
     */
    BOOL operator < (const Ant_TimeSpan& ts) const;

    /**
     * operator <=
     */
    BOOL operator <= (const Ant_TimeSpan& ts) const;

    /**
     * operator >
     */
    BOOL operator > (const Ant_TimeSpan& ts) const;

    /**
     * operator >=
     */
    BOOL operator >= (const Ant_TimeSpan& ts) const;

    /**
     * operator +=
     */
    Ant_TimeSpan& operator += (const Ant_TimeSpan& ts);

    /**
     * operator -=
     */
    Ant_TimeSpan& operator -= (const Ant_TimeSpan& ts);

public:

    Ant_TimeSpan();
    Ant_TimeSpan(LONGLONG ticks);
    LONGLONG getTicks() const {return m_ticks;}
    static LONGLONG timeToTicks(INT h, INT mi, INT s);
    static LONGLONG dayTimeToTicks(INT d, INT h, INT mi, INT s, INT ms);
    static LONGLONG dateToTicks(INT y, INT m, INT d);
    static ULONGLONG antTimeToTicks(const Ant_TimeStruct&);
    static Ant_TimeStruct ticksToAntTime(const ULONGLONG&);
    static ULONGLONG add(const Ant_TimeStruct&, const Ant_TimeSpan&);
    static ULONGLONG subtract(const Ant_TimeStruct&, const Ant_TimeSpan&);
    static LONGLONG subtract(const Ant_TimeStruct&, const Ant_TimeStruct&);
    static BOOL isLeapYear(INT y);
    static INT dayOfWeek(INT y, INT m, INT d);

private:

    LONGLONG m_ticks;

};

/**
* operator for Ant_TimeSpan = Ant_TimeSpan + Ant_TimeSpan
*/
ANTLIB_API Ant_TimeSpan operator + (const Ant_TimeSpan& lhs, const Ant_TimeSpan& rhs);

/**
* operator for Ant_TimeSpan = Ant_TimeSpan - Ant_TimeSpan
*/
ANTLIB_API Ant_TimeSpan operator - (const Ant_TimeSpan& lhs, const Ant_TimeSpan& rhs);

/**
 *  class Ant_Time
 *
 * The class to get local/system time.
 *
 */
class ANTLIB_API Ant_Time : public virtual Ant_Object
{
public:

    friend ANTLIB_API Ant_TimeSpan operator - (const Ant_Time& lhs, const Ant_Time& rhs);

    /**
     * Construct an zero date&time
     *
     */
    Ant_Time();

    /**
     * Construct an appointted date&time
     *
     * @param y The year. The valid values for this member are 1601 through 30827.
     * @param m The month. This member can be one of the following values. (1-January ~ 12-December)
     * @param d The day of the month. The valid values for this member are 1 through 31.
     * @param h The hour. The valid values for this member are 0 through 23.
     * @param mi The minute. The valid values for this member are 0 through 59.
     * @param s The second. The valid values for this member are 0 through 59.
     * @param ms The millisecond. The valid values for this member are 0 through 999.
     */
    Ant_Time(INT y, INT m, INT d, INT h = 0, INT mi = 0, INT s = 0, INT ms = 0);

    /**
     * Copy constructor
     */
    Ant_Time(const Ant_Time&);

    /**
     * operator =
     */
    Ant_Time& operator =(const Ant_Time&);

    /**
     * Destructor
     *
     */
    ~Ant_Time();

    /**
     * Get the year of the date&time which storaged in this class.
     *
     * @return INT year of the date&time
     */
    INT getYear() const {return (INT)(m_cTime.wYear);}

    /**
     * Get the month of the date&time which storaged in this class.
     *
     * @return INT month of the date&time (1-January ~ 12-December)
     */
    INT getMonth() const {return (INT)(m_cTime.wMonth);}

    /**
     * Get the week of the date&time which storaged in this class.
     *
     * @return INT week of the date&time (0-Sunday ~ 6-Saturday)
     */
    INT getDayOfWeek() const {return (INT)(m_cTime.wDayOfWeek);}

    /**
     * Get the day of the date&time which storaged in this class.
     *
     * @return INT day of the date&time (1 ~ 31)
     */
    INT getDay() const {return (INT)(m_cTime.wDay);}

    /**
     * Get the hour of the date&time which storaged in this class.
     *
     * @return INT hour of the date&time (0 ~ 23)
     */
    INT getHour() const {return (INT)(m_cTime.wHour);}

    /**
     * Get the minute of the date&time which storaged in this class.
     *
     * @return INT minute of the date&time (0 ~ 59)
     */
    INT getMinute() const {return (INT)(m_cTime.wMinute);}

    /**
     * Get the second of the date&time which storaged in this class.
     *
     * @return INT second of the date&time (0 ~ 59)
     */
    INT getSecond() const {return (INT)(m_cTime.wSecond);}

    /**
     * Get the millisecond of the date&time which storaged in this class.
     *
     * @return INT millisecond of the date&time (0 ~ 999)
     */
    INT getMillisecond() const {return (INT)(m_cTime.wMilliseconds);}

    /**
     * Set the date&time which storaged in this class.
     *
     */
    VOID set(INT y, INT m, INT d, INT h = 0, INT mi = 0, INT s = 0, INT ms = 0);

    /**
     * Set the time which storaged in this class.
     *
     * @param h The hour. The valid values for this member are 0 through 23.
     * @param mi The minute. The valid values for this member are 0 through 59.
     * @param s The second. The valid values for this member are 0 through 59.
     * @param ms The millisecond. The valid values for this member are 0 through 999.
     */
    VOID setTime(INT h, INT mi, INT s, INT ms = 0);

    /**
     * Set the date which storaged in this class.
     *
     * @param y The year. The valid values for this member are 1601 through 30827.
     * @param m The month. This member can be one of the following values. (1-January ~ 12-December)
     * @param d The day of the month. The valid values for this member are 1 through 31.
     */
    VOID setDate(INT y, INT m, INT d);

    /**
     * Storage the Current Local Time.
     *
     */
    VOID getNow();

    /**
     * Storage the Current System Time (UTC).
     *
     */
    VOID getUTCNow();

    /**
     * Get the current cpu tickcount.
     *
     */
    DWORD getTickCount();

    /**
     * operator == to compare two Ant_Time whether equal.
     */
    BOOL operator == (const Ant_Time&) const;

    /**
     * operator != to compare two Ant_Time whether not equal.
     */
    BOOL operator != (const Ant_Time&) const;

    /**
     * operator += to add a Ant_TimeSpan.
     */
    Ant_Time& operator += (const Ant_TimeSpan& ts);

    /**
     * operator -= to subtract a Ant_TimeSpan.
     */
    Ant_Time& operator -= (const Ant_TimeSpan& ts);

    BOOL operator <  (const Ant_Time&) const;
    BOOL operator <= (const Ant_Time&) const;
    BOOL operator >  (const Ant_Time&) const;
    BOOL operator >= (const Ant_Time&) const;

    BOOL isValidTimeDate() const;
    BOOL isValidTime() const;
    BOOL isValidDate() const;

private:

    VOID set(ANT_SYSTEMTIME&);
    Ant_TimeStruct m_cTime;

};

/**
 * operator for Ant_Time = Ant_Time + Ant_TimeSpan
 */
ANTLIB_API Ant_Time operator + (const Ant_Time& lhs, const Ant_TimeSpan& rhs);

/**
 * operator for Ant_Time = Ant_Time - Ant_TimeSpan
 */
ANTLIB_API Ant_Time operator - (const Ant_Time& lhs, const Ant_TimeSpan& rhs);

/**
 * operator for Ant_TimeSpan = Ant_Time - Ant_Time
 */
ANTLIB_API Ant_TimeSpan operator - (const Ant_Time& lhs, const Ant_Time& rhs);

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_TIME_H

/*---------------------------------------------------------------------------*/
/* EOF */
