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
 */

#include <ctype.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <qevent.h>
#include <qscrbar.h>
#include <qtimer.h>
#include <qdatetm.h>
#include <qpainter.h>
#include <qdrawutil.h>
#include <qstyle.h>

#include "calendar.h"

#define CALENDAR_WIDTH  130
#define TITLE_HEIGHT    20
#define TITLE_FONT_SIZE 10
#define ROW_HEIGHT      15
#define BLOCK_SIZE      13 // BLOCK_SIZE must be ROW_HEIGHT - 2 <- GAP
#define FONT_SIZE       8

#define INITIAL_TIME 750
#define REPEAT_TIME 100

static char* months[] = { "Jan", "Feb", "Mar", "Apr",
  "May", "Jun", "Jul", "Aug", "Sep", "Oct",
  "Nov", "Dec" 
};

// Calendar

Calendar::Calendar(QWidget* p, DateInput* di)
:QWidget(p)
{
  dateinput=di;

  curyear=98;
  curmonth=9;
  curday=25;
  calrow=calcol=startdow=stopdow=numrows=-1;
  direction=initial=0;
  setMouseTracking(TRUE);
  setFixedWidth(CALENDAR_WIDTH);

  timer=new QTimer(this);
  CHECK_PTR(timer);
  connect(timer,SIGNAL(timeout()),SLOT(timerEvent()));

  tip=new CalendarTip(this,this);
  CHECK_PTR(tip);
}

void Calendar::show()
{
  // get curyear, curmonth, curday from string
  QString str=dateinput->getDate();
  dateinput->validator()->fixup(str);
  dateinput->setDate(str);
  char* tempstr=strdup(dateinput->getDate());
  curmonth=atoi(strtok(tempstr,"/"))-1;
  curday=atoi(strtok(NULL,"/"));
  curyear=atoi(strtok(NULL,"/"));
  if (curyear>=1900) curyear-=1900;
  free(tempstr);

  recalc();
  raise();
  QWidget::show();
}

void Calendar::hide()
{
  QWidget::hide();
}

const QDate Calendar::getQDate()
{
    QDate   retVal = myQDate;
    return (const QDate) retVal;
}

void Calendar::recalc(void)
{
  // Calculate some time stuff...
  struct tm tm1;
  tm1.tm_sec=0;
  tm1.tm_min=0;
  tm1.tm_hour=12;
  tm1.tm_mday=1;
  tm1.tm_mon=curmonth;
  tm1.tm_year=curyear;
  time_t temp=mktime(&tm1);
  struct tm* ptr=localtime(&temp);
  startdow=ptr->tm_wday;
  int nextmonth=curmonth+1;
  int nextyear=curyear;
  if (nextmonth>11) {
    nextmonth=0;
    nextyear++;
  }
  tm1.tm_sec=0;
  tm1.tm_min=0;
  tm1.tm_hour=12;
  tm1.tm_mday=1;
  tm1.tm_mon=nextmonth;
  tm1.tm_year=nextyear;
  temp=mktime(&tm1);
  temp-=3600*24;	// 1 day
  ptr=localtime(&temp);
  numdaysinmonth=ptr->tm_mday;
  stopdow=ptr->tm_wday;
  numrows=(numdaysinmonth+startdow-1)/7;

  // Get day coloring information, if present
  for (int i=0;i<31;i++)
    daycolor[i]=NoColor;
  emit(classifyRequest(curmonth,curyear,daycolor));

  resize(width(),numrows*ROW_HEIGHT+TITLE_HEIGHT*2+ROW_HEIGHT);
}

int Calendar::mouseLeftArrow(QMouseEvent* qme)
{
  if (qme->x()>=10 && qme->x()<=20 && 
			qme->y()>=(TITLE_HEIGHT-10)/2 && 
			qme->y()<=TITLE_HEIGHT-(TITLE_HEIGHT-10)/2 )
    return 1;
  return 0;
}

int Calendar::mouseRightArrow(QMouseEvent* qme)
{
  if (qme->x()>=width()-20 && qme->x()<=width()-10 && 
			qme->y()>=(TITLE_HEIGHT-10)/2 && 
			qme->y()<=TITLE_HEIGHT-(TITLE_HEIGHT-10)/2 )
    return 1;
  return 0;
}

void Calendar::paintEvent(QPaintEvent*)
{
  QPainter paint;
  paint.begin(this);
  QColorGroup g=colorGroup();
  paint.setPen(g.text());
  paint.fillRect(0,0,width(),height(),white);
  paint.fillRect(0,0,width(),TITLE_HEIGHT,yellow);
  paint.setPen(black);
  paint.drawLine(0,0,width()-1,0);
  paint.drawLine(0,0,0,height()-1);
  paint.drawLine(width()-1,0,width()-1,height()-1);
  paint.drawLine(0,height()-1,width()-1,height()-1);

  QFont font("Helvetica",TITLE_FONT_SIZE);
  font.setBold(TRUE);
  paint.setFont(font);
  char txt[30];
  sprintf(txt,"%s, %i",months[curmonth],curyear+1900);
  paint.drawText(0,0,width(),TITLE_HEIGHT,AlignCenter,txt,strlen(txt));
  QFont font1("Helvetica",FONT_SIZE);
  paint.setFont(font1);
  int w=width()/7;
  static char* dow[]={"Su","M","Tu","W","Th","F","Sa"};
  for (int j=0;j<7;j++) {
    paint.drawText(j*w+2,TITLE_HEIGHT,w,ROW_HEIGHT,AlignCenter,dow[j],
      strlen(dow[j]));
  }
  for (int i=1;i<=numdaysinmonth;i++) {
    char txt[3];
    sprintf(txt,"%i",i);
    if (daycolor[i-1] == Color1) {
      paint.fillRect(((i+startdow-1)%7)*w+4,((i+startdow-1)/7)*ROW_HEIGHT+
        TITLE_HEIGHT+ROW_HEIGHT+1,BLOCK_SIZE,BLOCK_SIZE,gray);
    }
    if (daycolor[i-1] == Color2) {
      paint.fillRect(((i+startdow-1)%7)*w+4,((i+startdow-1)/7)*ROW_HEIGHT+
        TITLE_HEIGHT+ROW_HEIGHT+1,BLOCK_SIZE,BLOCK_SIZE,red);
    }
    if (i==curday) {
      paint.drawRect(((i+startdow-1)%7)*w+3,((i+startdow-1)/7)*ROW_HEIGHT+
        TITLE_HEIGHT+ROW_HEIGHT,ROW_HEIGHT,ROW_HEIGHT);
    }
    paint.drawText(((i+startdow-1)%7)*w+3, ((i+startdow-1)/7)*ROW_HEIGHT+
      TITLE_HEIGHT+ROW_HEIGHT,ROW_HEIGHT,ROW_HEIGHT,AlignCenter,txt,
      strlen(txt));
  }
  QPointArray points(3);
  points.setPoints(3,10,TITLE_HEIGHT/2,20,TITLE_HEIGHT-(TITLE_HEIGHT-10)/2,
    20,(TITLE_HEIGHT-10)/2);
  paint.setBrush(black);
  paint.drawPolygon(points,TRUE);
  points.setPoints(3,width()-10,TITLE_HEIGHT/2,width()-20,TITLE_HEIGHT-
    (TITLE_HEIGHT-10)/2,width()-20,(TITLE_HEIGHT-10)/2);
  paint.drawPolygon(points,TRUE);
  paint.end();
}

void Calendar::mousePressEvent(QMouseEvent* qme)
{
  char str[20];
  if (mouseLeftArrow(qme)) {
    curmonth--;
    if (curmonth<0) {
      curmonth=11;
      curyear--;
    }
    direction=0;
    initial=1;
    timer->start(INITIAL_TIME);
    myQDate.setYMD(curyear+1900, curmonth+1, curday);
    sprintf(str,"%i/%i/%i",curmonth+1,curday,curyear+1900);
    dateinput->setDate(str);
    recalc();
    repaint();
  } else if (mouseRightArrow(qme)) {
    curmonth++;
    if (curmonth>11) {
      curmonth=0;
      curyear++;
    }
    direction=1;
    initial=1;
    timer->start(INITIAL_TIME);
    myQDate.setYMD(curyear+1900, curmonth+1, curday);
    sprintf(str,"%i/%i/%i",curmonth+1,curday,curyear+1900);
    dateinput->setDate(str);
    recalc();
    repaint();
  } else {
    int row=int(qme->y()>TITLE_HEIGHT+ROW_HEIGHT?(qme->y()-(TITLE_HEIGHT+
      ROW_HEIGHT))/ROW_HEIGHT:-1);
    int col=int(qme->x()/(width()/7));
    if (!((row==0 && col<startdow) || (row>numrows) || (col>6) ||
      (row==numrows && col>stopdow)) && col>=0 && row>=0) {
      curday=row*7+col+1-startdow;
      char text[20];
      myQDate.setYMD(curyear+1900, curmonth+1, curday);
      sprintf(text,"%i/%i/%i",curmonth+1,curday,curyear+1900);
      dateinput->setDate(text);
      /*
      cerr<<curmonth+1<<"/"<<curday<<"/"<<curyear+1900<<" selected!"<<endl;
      cerr<<"count="<<count()<<endl;
      */
      calrow=calcol=-1;
      hide();
    }
  }
}

void Calendar::mouseMoveEvent(QMouseEvent* qme)
{
  int row=int(qme->y()>TITLE_HEIGHT+ROW_HEIGHT?(qme->y()-(TITLE_HEIGHT+
    ROW_HEIGHT))/ROW_HEIGHT:-1);
  int col=int(qme->x()/(width()/7));
  if ((row==0 && col<startdow) || (row>numrows) || (col>6) ||
    (row==numrows && col>stopdow)) {
    row=col=-99;
  }
  QPainter paint;
  paint.begin(this);
  if (row!=calrow || col!=calcol) {
    if (curday == calrow*7+calcol+1-startdow) {
      paint.setPen(black);
    } else {
      paint.setPen(white);
    }
    paint.drawRect(calcol*(width()/7)+3,calrow*ROW_HEIGHT+TITLE_HEIGHT+
      ROW_HEIGHT,ROW_HEIGHT,ROW_HEIGHT);
    if (row>=0 && col>=0) { 
      calrow=row;
      calcol=col;
      QColorGroup g=colorGroup();
      int x=calcol*(width()/7)+3;
      int y=calrow*ROW_HEIGHT+TITLE_HEIGHT+ROW_HEIGHT;
      paint.setPen(g.midlight());
      paint.drawLine(x,y,x,y+ROW_HEIGHT-1);
      paint.drawLine(x,y,x+ROW_HEIGHT-1,y);
      paint.setPen(g.dark());
      paint.drawLine(x+ROW_HEIGHT-1,y,x+ROW_HEIGHT-1,y+ROW_HEIGHT-1);
      paint.drawLine(x,y+ROW_HEIGHT-1,x+ROW_HEIGHT-1,y+ROW_HEIGHT-1);
    } else {
      calrow=calcol=-99;
    }
  }
  paint.end();
}

void Calendar::mouseReleaseEvent(QMouseEvent*)
{
  timer->stop();
}

void Calendar::timerEvent()
{
  if (initial) {
    initial=0;
    timer->start(REPEAT_TIME);
  }
  if (direction) {
    curmonth++;
    if (curmonth>11) {
      curmonth=0;
      curyear++;
    }
  } else {
    curmonth--;
    if (curmonth<0) {
      curmonth=11;
      curyear--;
    }
  }
  char str[20];
  sprintf(str,"%i/%i/%i",curmonth+1,curday,curyear+1900);
  dateinput->setDate(str);
  recalc();
  repaint();
}

void Calendar::getDayLabel(int m, int d, int y, char* t)
{
  // default to null if not connected...
  strcpy(t,"");
  emit(dayLabel(m,d,y,t));
}

// CalendarTip

CalendarTip::CalendarTip(QWidget* p, Calendar* c)
:QToolTip(p)
{
  cal=c;
}

void CalendarTip::maybeTip(const QPoint& p)
{
  int row=int(p.y()>TITLE_HEIGHT+ROW_HEIGHT?(p.y()-(TITLE_HEIGHT+ROW_HEIGHT))/
    ROW_HEIGHT:-1);
  int col=int(p.x()/(cal->width()/7));
  int curday;

  if (!((row==0 && col<cal->startdow) || (row>cal->numrows) || (col>6) ||
    (row==cal->numrows && col>cal->stopdow)) && col>=0 && row>=0) {
    curday=row*7+col+1-cal->startdow;
    QRect rect(p.x()-5,p.y()-5,10,10);
    char tiptext[100],text[100];
    cal->getDayLabel(cal->curmonth,curday,cal->curyear+1900,text);
    if (strcmp(text,""))
      sprintf(tiptext,"%s %i, %i\n%s",months[cal->curmonth],curday,
        cal->curyear+1900,text);
    else
      sprintf(tiptext,"%s %i, %i",months[cal->curmonth],curday,
        cal->curyear+1900);
    tip(rect,tiptext);
  } 
}

// DateValidator

DateValidator::DateValidator(QWidget* p)
:QValidator(p)
{
}

QValidator::State DateValidator::validate(QString& str, int&) const
{
  int tempmon=1;
  int tempday=1;
  int tempyear=1970;

  // Every character must be either a digit or a slash
  for (unsigned int i=0;i<strlen(str);i++)
    if (!str.at(i).isDigit() && str.at(i)!='/')
      return Invalid;

  // Must have exactly two slashes
  char* firstslash=strchr(str,'/');
  if (firstslash==NULL) 
    return Valid;
  else {
    tempmon=atoi(str);
  }
  char* secondslash=strchr(firstslash+1,'/');
  if (secondslash==NULL)
    return Valid;
  else {
    tempday=atoi(firstslash+1);
    tempyear=atoi(secondslash+1);
  }

  // Month, day and year must be in an acceptable range
  if (tempmon>12 || tempday>31 || tempyear>2100)
    return Invalid;
  if (tempmon<1 || tempday<1 || tempyear<1970)
    return Valid;

  return Acceptable;
}

void DateValidator::fixup(QString& str)
{
  int tempmon=1;
  int tempday=1;
  int tempyear=1970;

  char* firstslash=strchr(str,'/');
  if (firstslash != NULL) {
    tempmon=atoi(str);
    char* secondslash=strchr(firstslash+1,'/');
    if (secondslash != NULL) {
      tempday=atoi(firstslash+1);
      tempyear=atoi(secondslash+1);
    }
  }
  if (tempmon<=0) tempmon=1;
  if (tempday<1) tempday=1;
  if (tempyear<1970) tempyear=1970;
  str.sprintf("%i/%i/%i",tempmon,tempday,tempyear);
}

// DateInput

DateInput::DateInput(QWidget* p, const char* txt)
:QWidget(p)
{
  text=new QLineEdit(this);
  CHECK_PTR(text);
  //text->setValidator(new DateValidator(this));
  connect(text,SIGNAL(returnPressed()),SLOT(slotNewText()));

  cal=new Calendar(p,this);
  CHECK_PTR(cal);
  cal->hide();
  connect(cal,SIGNAL(classifyRequest(int,int,char*)),
    SLOT(slotClassifyRequest(int,int,char*)));
  connect(cal,SIGNAL(dayLabel(int,int,int,char*)),
    SLOT(slotDayLabel(int,int,int,char*)));

  setDate(txt);
}

DateInput::DateInput(QWidget* p, const QDate sdate)
:QWidget(p)
{
  text=new QLineEdit(this);
  CHECK_PTR(text);
  //text->setValidator(new DateValidator(this));
  connect(text,SIGNAL(returnPressed()),SLOT(slotNewText()));

  cal=new Calendar(p,this);
  CHECK_PTR(cal);
  cal->hide();
  connect(cal,SIGNAL(classifyRequest(int,int,char*)),
    SLOT(slotClassifyRequest(int,int,char*)));
  connect(cal,SIGNAL(dayLabel(int,int,int,char*)),
    SLOT(slotDayLabel(int,int,int,char*)));

  char txt[16];
  sprintf(txt, "%d/%d/%d", sdate.month(), sdate.day(), sdate.year());
  setDate(txt);
}


void DateInput::setDate(const char* str)
{
    QString s(str);

    if (str == (const char *) NULL)
        s = QDate::currentDate().toString("MM/dd/yyyy");

    text->setText(s);
    emit(dateChanged());
}

void DateInput::setDate(const QDate newDate)
{
  char str[128];
  sprintf(str, "%d/%d/%d", newDate.month(), newDate.day(), newDate.year());
  text->setText(str);
  emit(dateChanged());
}

const char* DateInput::getDate(void)
{
  return text->text();
}

const QDate DateInput::getQDate()
{
    int     tmpPos = 0;
    char    *tmpStr = new char[1024];
    char    *part;
    int     Y = 1900, M = 1, D = 1;
    QDate   retVal;

    strcpy(tmpStr, text->text());
    part = strsep(&tmpStr, "/");
    if (part) {
        M = atoi(part);
        part = strsep(&tmpStr, "/");
        if (part) {
            D = atoi(part);
            part = strsep(&tmpStr, "/");
            if (part) Y = atoi(part);
        }
    }

    retVal.setYMD(Y,M,D);
    return (const QDate) retVal;
}

void DateInput::slotClassifyRequest(int m,int y,char* c)
{
  emit(classifyRequest(m,y,c));
}

void DateInput::slotDayLabel(int m, int d, int y, char* t)
{
  emit(dayLabel(m,d,y,t));
}

void DateInput::slotNewText()
{
  QRect rect = geometry();
  cal->setGeometry(rect.x(),rect.y()+height(),rect.width(),rect.height());
  cal->show();
}

void DateInput::paintEvent(QPaintEvent*)
{
  QPainter paint;
  paint.begin(this);
  qDrawShadePanel(&paint,0,0,width(),height(),colorGroup(),FALSE,1,NULL);
  QRect r(width() - 19, 5, height() - 12, height() - 12);
  style().drawPrimitive(QStyle::PE_ArrowDown, &paint, r, colorGroup(), FALSE);
  qDrawShadeLine(&paint,width()-18,height()-7,width()-7,height()-7,
    colorGroup(),FALSE,1,1);
  paint.end();
}

void DateInput::resizeEvent(QResizeEvent*)
{
  text->setGeometry(2,2,width()-27,height()-4);
}

void DateInput::mousePressEvent(QMouseEvent* qme)
{
  int x = qme->x();
  int y = qme->y();
  if (x > width()-20 && x < width()-4 && y > 5 && y < height()-5) {
    if (cal->isVisible()) {
      cal->hide();
    } else {
      QRect rect = geometry();
      cal->setGeometry(rect.x(),rect.y()+height(),rect.width(),rect.height());
      cal->show();
    }
  }
}


