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

#include <stdio.h>
#include <time.h>

#include "holiday.h"
#include "worktime.h"

// WorkTime

WorkTime::WorkTime(int y)
{
  year=y;
  if (year>1900) year-=1900;
  // Classify each day of the year
  for (int mon=0;mon<12;mon++) {
    struct tm temptime;
    temptime.tm_sec=temptime.tm_min=temptime.tm_hour=0;
    temptime.tm_mday=1;
    temptime.tm_mon=mon;
    temptime.tm_year=year;
    temptime.tm_wday=temptime.tm_yday=0;
    temptime.tm_isdst=-1;
    time_t temp=mktime(&temptime);
    struct tm* ptr=localtime(&temp);
    int dow=ptr->tm_wday;
    daysinmonth[mon]=numDaysInMonth(mon,year);
    for (int i=0;i<daysinmonth[mon];i++) {
      days[mon][i].dow=dow++;
      if (dow>6) dow=0;
      if (days[mon][i].dow==0 || days[mon][i].dow==6)
        days[mon][i].dtype=Weekend;
      else
        days[mon][i].dtype=WorkDay;
    }
  }
  // Assign the holidays
  // Check New Year's Day of this year and next year
  workHoliday(NewYearsDay(year+1900));
  workHoliday(MartinLutherKingDay(year+1900));
  workHoliday(WashingtonsBirthday(year+1900));
  workHoliday(GoodFriday(year+1900));
  workHoliday(MemorialDay(year+1900));
  workHoliday(IndependenceDay(year+1900));
  workHoliday(LaborDay(year+1900));
  workHoliday(ThanksgivingDay(year+1900));
  workHoliday(ChristmasDay(year+1900));
  if (days[11][30].dow == Fri) {
    days[11][30].dtype=HolidayObserved;
    days[11][30].key=_NewYearsDay;
  }
}

int WorkTime::monthLength(int mon)
{
  return daysinmonth[mon];
}

int WorkTime::dayType(int mon, int day)
{
  return days[mon][day].dtype;
}

int WorkTime::isWorkDay(int mon, int day)
{
  if (days[mon][day].dtype == WorkDay)
    return 1;
  return 0;
}

int WorkTime::isHoliday(int mon, int day)
{
  if (days[mon][day].dtype == Holiday || 
      days[mon][day].dtype == HolidayObserved)
    return 1;
  return 0;
}

int WorkTime::isWeekend(int mon, int day)
{
  if (days[mon][day].dtype == Weekend)
    return 1;
  return 0;
}

const char* WorkTime::dayText(int mon, int day)
{
  if (days[mon][day].dtype == Holiday)
    return holidayText[days[mon][day].key];
  if (days[mon][day].dtype == HolidayObserved) {
    sprintf(workstr,"%s (observed)",holidayText[days[mon][day].key]);
    return workstr;
  }
  return "";
}

int WorkTime::dayNumber(int mon, int day)
{
  if (days[mon][day].dtype == WorkDay)
    return days[mon][day].key+1;
  return -1;
}

void WorkTime::numberWorkDays(void)
{
  int count=0;
  for (int i=0;i<12;i++) {
    for (int j=0;j<daysinmonth[i];j++) {
      if (days[i][j].dtype == WorkDay)
        days[i][j].key=count++;
    }
  }
}

void WorkTime::dump(void)
{
  for (int mon=0;mon<12;mon++) {
    if (mon>0) printf("\n");
    for (int j=0;j<(int)days[mon][0].dow;j++)
      printf(" ");
    for (int j=0;j<daysinmonth[mon];j++) {
      if (days[mon][j].dtype==WorkDay)
        printf("T");
      if (days[mon][j].dtype==Weekend)
        printf("W");
      if (days[mon][j].dtype==Holiday)
        printf("H");
      if (days[mon][j].dtype==HolidayObserved)
        printf("O");
      if (days[mon][j].dow==6)
        printf("\n");
    }
  }
  printf("\n");
  printf("sizeof (DAY) = %i\n",sizeof(DAY));
}

int WorkTime::numDaysInMonth(int mon, int year)
{
  if (mon==0) return 31;	// January
  if (mon==1) {			// February
    if (year%4 == 0) {
      if (year%100 == 0) return 28;
      else return 29;
    }
    return 28;
  }
  if (mon==2) return 31;	// March
  if (mon==3) return 30;	// April
  if (mon==4) return 31;	// May
  if (mon==5) return 30;	// June
  if (mon==6) return 31;	// July
  if (mon==7) return 31;	// August
  if (mon==8) return 30;	// September
  if (mon==9) return 31;	// October
  if (mon==10) return 30;	// November
  if (mon==11) return 31;	// December
  return -1;  // error
}

void WorkTime::workHoliday(struct MonthDay md)
{
  int actday=md.mday;
  int actmon=md.mon;
  int extra=0;
  if (days[actmon][actday-1].dow == Sat) {
    /* if date is on a Saturday, holiday is on the previous Friday */
    extra=1;
    actday--;
    if (actday == 0) { 
      if (actmon>0) actday=daysinmonth[--actmon];
      else extra=0;
    }
  } else if (days[actmon][actday-1].dow == Sun) {
    /* if date is on a Sunday, holiday is on the next Monday */
    extra=1;
    actday++;
    if (actday > daysinmonth[actmon]) {
      if (actmon<11) {
        actday=1;
        actmon++;
      } else
        extra=0;
    }
  }
  if (days[md.mon][md.mday-1].dow != Sun && days[md.mon][md.mday-1].dow != Sat) {
    days[md.mon][md.mday-1].dtype=Holiday;
    days[md.mon][md.mday-1].key=md.key;
  }
  if (extra) {
    days[actmon][actday-1].dtype=HolidayObserved;
    days[actmon][actday-1].key=md.key;
  }
}


