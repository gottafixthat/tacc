/*
 *
 * Copyright (c) 1998,1999, Mark Jackson.  
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as included in
 * the file COPYING in the main directory for this package.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef __HOLIDAY_H
#define __HOLIDAY_H

// Max chars in holiday string
#define MAXHOLIDAY 50

typedef enum { _NewYearsDay, _MartinLutherKingDay, _WashingtonsBirthday,
  _GoodFriday, _Easter, _MemorialDay, _IndependenceDay, _LaborDay,
  _ThanksgivingDay, _ChristmasDay } HolidayType ;

extern const char* holidayText[];

struct MonthDay {
  short mon;      /* Month 0-11 */
  short mday;     /* Day of month 1-31 */
  short key;      /* Holiday key */
};

struct MonthDay NewYearsDay(int year);
struct MonthDay MartinLutherKingDay(int year);
struct MonthDay WashingtonsBirthday(int year);
struct MonthDay GoodFriday(int year);
struct MonthDay Easter(int year);	/* valid years 326 to 2599 A.D. */
struct MonthDay MemorialDay(int year);
struct MonthDay IndependenceDay(int year);
struct MonthDay LaborDay(int year);
struct MonthDay ThanksgivingDay(int year);
struct MonthDay ChristmasDay(int year);

#endif

