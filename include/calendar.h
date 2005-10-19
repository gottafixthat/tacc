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
 * Modified on May 5, 2000 by R. Marc Lewis <marc@blarg.net> to work 
 * with the QDate class.
 *
 * Modified 10/2/2005 by Jeff Woods <jcwoods@bellsouth.net>:
 * - fixed sources to build with qt3 (qt-3.3.4, in particlular...)
 * - added default of current date if no date specified in DateInput
 *   constructor
 */

#ifndef __CALENDAR_H
#define __CALENDAR_H

#include <qlineedit.h>
#include <qlistbox.h>
#include <qtooltip.h>
#include <qvalidator.h>
#include <qwidget.h>
#include <qdatetm.h>

class Calendar;
class CalendarTip;
class DateInput;
class QMouseEvent;
class QPaintEvent;
class QResizeEvent;

typedef enum { NoColor, Color1, Color2, Color3 } DayColors;

class Calendar : public QWidget {
  Q_OBJECT
public:
  Calendar(QWidget*, DateInput*);
  void show();
  void hide();
  const QDate getQDate();
signals:
  void classifyRequest(int mon, int yr, char*);
  void dayLabel(int, int, int, char*);
protected:
  void paintEvent(QPaintEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
protected slots:
  void timerEvent();
private:
  void getDayLabel(int, int, int, char*);
  void recalc(void);
  int mouseLeftArrow(QMouseEvent*);
  int mouseRightArrow(QMouseEvent*);

  int initial,direction;
  int curyear,curmonth,curday;
  int calrow,calcol,startdow,stopdow,numrows;
  int numdaysinmonth;
  char daycolor[31];
  QTimer* timer;
  DateInput* dateinput;
  CalendarTip* tip;

  QDate myQDate;

  friend class CalendarTip;
};

class CalendarTip : public QToolTip {
public:
  CalendarTip(QWidget*,Calendar*);
protected:
  void maybeTip(const QPoint&);
private:
  Calendar* cal;
};

class DateValidator : public QValidator {
public:
  DateValidator(QWidget*);
  State validate(QString&,int&) const;
  void fixup(QString&);
};

class DateInput : public QWidget {
  Q_OBJECT
public:
  DateInput(QWidget*,const char* = NULL);
  DateInput(QWidget*, const QDate);
  const char* getDate(void);
  const QDate getQDate();
  DateValidator* validator(void) {return dv;}
  void setDate(const char*);
  void setDate(const QDate);
signals:
  void classifyRequest(int,int,char*);
  void dayLabel(int, int, int, char*);
  void dateChanged();
protected slots:
  void slotDayLabel(int, int, int, char*);
  void slotClassifyRequest(int,int,char*);
  void slotNewText();
protected:
  void paintEvent(QPaintEvent*);
  void resizeEvent(QResizeEvent*);
  void mousePressEvent(QMouseEvent*);
private:
  Calendar* cal;
  DateValidator* dv;
  QLineEdit* text;
};

#endif


