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
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "holiday.h"

const char* holidayText[] = {
  "New Year's Day",
  "Martin Luther King Day",
  "Washington's Birthday",
  "Good Friday",
  "Easter",
  "Memorial Day",
  "Independence Day",
  "Labor Day",
  "Thanksgiving Day",
  "Christmas Day"
};

struct MonthDay NewYearsDay(int)
{
  struct MonthDay md;
  md.mon=0;
  md.mday=1;
  md.key=_NewYearsDay;
  return md;
}

struct MonthDay MartinLutherKingDay(int year)
{
  struct MonthDay md;
  /* Third monday in January */
#ifdef _DEBUG
printf("Martin Luther King Day calculation for year %i:\n",year);
#endif
  if (year>1900) year-=1900;
  struct tm thistime;
  thistime.tm_year=year;
  thistime.tm_mon=0;
  thistime.tm_mday=1;
  thistime.tm_hour=thistime.tm_min=thistime.tm_sec=0;
  thistime.tm_wday=thistime.tm_yday=0;
  thistime.tm_isdst=-1;
  time_t time=mktime(&thistime);
  struct tm* ptr=localtime(&time);
  int dtfm=1-ptr->tm_wday;
  if (dtfm<0) dtfm+=7;
  md.mday=dtfm + 15;
  md.mon=0;
  md.key=_MartinLutherKingDay;
#ifdef _DEBUG
printf("  RESULT month=%i day=%i\n",md.mon+1,md.mday);
#endif
  return md;
}

struct MonthDay WashingtonsBirthday(int year)
{
  struct MonthDay md;
  /* Third monday in February */
#ifdef _DEBUG
printf("Washington's Birthday calculation for year %i:\n",year);
#endif
  if (year>1900) year-=1900;
  struct tm thistime;
  thistime.tm_year=year;
  thistime.tm_mon=1;
  thistime.tm_mday=1;
  thistime.tm_hour=thistime.tm_min=thistime.tm_sec=0;
  thistime.tm_wday=thistime.tm_yday=0;
  thistime.tm_isdst=-1;
  time_t time=mktime(&thistime);
  struct tm* ptr=localtime(&time);
  int dtfm=1-ptr->tm_wday;
  if (dtfm<0) dtfm+=7;
  md.mday=dtfm + 15;
  md.mon=1;
  md.key=_WashingtonsBirthday;
#ifdef _DEBUG
printf("  RESULT month=%i day=%i\n",md.mon+1,md.mday);
#endif
  return md;
}

struct MonthDay GoodFriday(int year)
{
  struct MonthDay md;
#ifdef _DEBUG
printf("GoodFriday calculation for year %i:\n",year);
#endif
  md=Easter(year);
  if (md.mday<3) {
    md.mon=2;
    md.mday=29+md.mday;
  } else {
    md.mday-=2;
  }
  md.key=_GoodFriday;
#ifdef _DEBUG
printf("  RESULT month=%i day=%i\n",md.mon+1,md.mday);
#endif
  return md;
}

/*
* EASTER
*
* Easter Sunday is defined as the Sunday following the Paschal Full
* Moon date for the year.  On the Gregorian calendar, it always falls
* on one of the 35 dates between March 22 and April 25.  This makes
* it hard to easily calculate.
*
* The following algorithm was found at 
*
* http://www.assa.org.au/edm.html
*
* for easily calculating the Easter holiday for a given year.
*
* All credit goes to Ronald W. Mallen, Adelaide, South Australia for 
* creating this algorithm (rwmallen@poboxes.com), which I have 
* implemented for the purposes of this package...
*/

char* EasterTable[19][6] = {
  { "A5" , "A12", "A13", "A14", "A15", "A16"},
  { "M25", "A1" , "A2" , "A3" , "A4" , "A5" },
  { "A13", "M21", "M22", "M23", "M24", "M25"},
  { "A2" , "A9" , "A10", "A11", "A12", "A13"},
  { "M22", "M29", "M30", "M31", "A1" , "A2" },
  { "A10", "A17", "A18", "A18", "M21", "M22"},
  { "M30", "A6" , "A7" , "A8" , "A9" , "A10"},
  { "A18", "M26", "M27", "M28", "M29", "M30"},
  { "A7" , "A14", "A15", "A16", "A17", "A18"},
  { "M27", "A3" , "A4" , "A5" , "A6" , "A7" },
  { "A15", "M23", "M24", "M25", "M26", "M27"},
  { "A4" , "A11", "A12", "A13", "A14", "A15"},
  { "M24", "M31", "A1" , "A2" , "A3" , "A4" },
  { "A12", "A18", "M21", "M22", "M23", "M24"},
  { "A1" , "A8" , "A9" , "A10", "A11", "A12"},
  { "M21", "M28", "M29", "M30", "M31", "A1" },
  { "A9" , "A16", "A17", "A17", "A18", "M21"},
  { "M29", "A5" , "A6" , "A7" , "A8" , "A9" },
  { "A17", "M25", "M26", "M27", "M28", "M29"}
};

struct MonthDay Easter(int year)
{
  struct MonthDay md;
  char* lookup=NULL;
#ifdef _DEBUG
printf("Easter calculation for year %i:\n",year);
#endif
  memset(&md,0,sizeof(struct MonthDay));
  /* STEP 1 */
  if (year >= 326 && year <= 1582) {
    lookup=EasterTable[year%19][0];
  } else if (year >= 1583 && year <= 1699) {
    lookup=EasterTable[year%19][1];
  } else if (year >= 1700 && year <= 1899) {
    lookup=EasterTable[year%19][2];
  } else if (year >= 1900 && year <= 2199) {
    lookup=EasterTable[year%19][3];
  } else if ((year >= 2200 && year <= 2299) || (year >= 2400 && year <= 2499)) {
    lookup=EasterTable[year%19][4];
  } else if ((year >= 2300 && year <= 2399) || (year >= 2500 && year <= 2599)) {
    lookup=EasterTable[year%19][5];
  }
#ifdef _DEBUG
printf("  lookup=%s\n",lookup);
#endif
  if (lookup) {
    int res1,res2,res3;
    /* STEP 2 */
    int dd=atoi(lookup+1);
    res1=(*lookup=='M')*((dd-19)%7) + 
         (*lookup=='A')*((dd+5)%7);
#ifdef _DEBUG
printf("  res1=%i\n",res1);
#endif
    int century=year/100;
    if (year<=1582) {
      res2=6-((century+1)%7);
    } else {
      if (century%4 != 0) {
        res2=5-((century-1)%4)*2;
      } else {
        res2=0;
      }
    }
#ifdef _DEBUG
printf("  res2=%i\n",res2);
#endif
    int y=year%100;
    int skip=y/4;
    res3=(y+skip)%7;
#ifdef _DEBUG
printf("  res3=%i\n",res3);
#endif
    /* STEP 3 */
    int dtns=7-((res1+res2+res3)%7);
    if (*lookup=='M' && dd+dtns<=31) {
      md.mon=2;
      md.mday=dd+dtns;
    } else {
      md.mon=3;
      md.mday=dd+dtns;
      if (md.mday>31) md.mday-=31;
    }
#ifdef _DEBUG
printf("  RESULT month=%i day=%i\n",md.mon+1,md.mday);
#endif
  }
  md.key=_Easter;
  return md;
}

struct MonthDay MemorialDay(int year)
{
  struct MonthDay md;
  /* Last monday in May */
#ifdef _DEBUG
printf("Memorial Day calculation for year %i:\n",year);
#endif
  if (year>1900) year-=1900;
  struct tm thistime;
  thistime.tm_year=year;
  thistime.tm_mon=4;
  thistime.tm_mday=31;
  thistime.tm_hour=thistime.tm_min=thistime.tm_sec=0;
  thistime.tm_wday=thistime.tm_yday=0;
  thistime.tm_isdst=-1;
  time_t time=mktime(&thistime);
  struct tm* ptr=localtime(&time);
  int dtlm=ptr->tm_wday-1;
  if (dtlm<0) dtlm+=7;
  md.mday=31 - dtlm;
  md.mon=4;
  md.key=_MemorialDay;
#ifdef _DEBUG
printf("  RESULT month=%i day=%i\n",md.mon+1,md.mday);
#endif
  return md;
}

struct MonthDay IndependenceDay(int)
{
  struct MonthDay md;
  md.mon=6;
  md.mday=4;
  md.key=_IndependenceDay;
  return md;
}

struct MonthDay LaborDay(int year)
{
  struct MonthDay md;
  /* First monday in September */
#ifdef _DEBUG
printf("Labor Day calculation for year %i:\n",year);
#endif
  if (year>1900) year-=1900;
  struct tm thistime;
  thistime.tm_year=year;
  thistime.tm_mon=8;
  thistime.tm_mday=1;
  thistime.tm_hour=thistime.tm_min=thistime.tm_sec=0;
  thistime.tm_wday=thistime.tm_yday=0;
  thistime.tm_isdst=-1;
  time_t time=mktime(&thistime);
  struct tm* ptr=localtime(&time);
  int dtfm=1-ptr->tm_wday;
  if (dtfm<0) dtfm+=7;
  md.mday=dtfm + 1;
  md.mon=8;
  md.key=_LaborDay;
#ifdef _DEBUG
printf("  RESULT month=%i day=%i\n",md.mon+1,md.mday);
#endif
  return md;
}

struct MonthDay ThanksgivingDay(int year)
{
  struct MonthDay md;
  /* Fourth thursday in November */
#ifdef _DEBUG
printf("Thanksgiving Day calculation for year %i:\n",year);
#endif
  if (year>1900) year-=1900;
  struct tm thistime;
  thistime.tm_year=year;
  thistime.tm_mon=10;
  thistime.tm_mday=1;
  thistime.tm_hour=thistime.tm_min=thistime.tm_sec=0;
  thistime.tm_wday=thistime.tm_yday=0;
  thistime.tm_isdst=-1;
  time_t time=mktime(&thistime);
  struct tm* ptr=localtime(&time);
  int dtft=4-ptr->tm_wday;
  if (dtft<0) dtft+=7;
  md.mday=dtft + 22;
  md.mon=10;
  md.key=_ThanksgivingDay;
#ifdef _DEBUG
printf("  RESULT month=%i day=%i\n",md.mon+1,md.mday);
#endif
  return md;
}

struct MonthDay ChristmasDay(int)
{
  struct MonthDay md;
  md.mon=11;
  md.mday=25;
  md.key=_ChristmasDay;
  return md;
}

