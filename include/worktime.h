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

#ifndef __WORKTIME_H
#define __WORKTIME_H

#include "holiday.h"

typedef enum {Sun, Mon, Tue, Wed, Thu, Fri, Sat} DOW;
typedef enum {WorkDay, Weekend, Holiday, HolidayObserved} DTYPE;

struct DAY {
  unsigned int dow:4;
  unsigned int dtype:4;
  unsigned char key;
};

class WorkTime {
public:
  WorkTime(int year);
  int monthLength(int mon);
  int dayType(int mon, int day);
  int isWorkDay(int mon, int day);
  int isWeekend(int mon, int day);
  int isHoliday(int mon, int day);
  const char* dayText(int mon, int day);
  int dayNumber(int mon, int day);
  void numberWorkDays(void);
  void dump(void);
private:
  int numDaysInMonth(int mon, int year);
  void workHoliday(struct MonthDay);

  int year;
  int daysinmonth[12];
  DAY days[12][31];
  char workstr[MAXHOLIDAY];
};

#endif

