/*
** $Id$
**
***************************************************************************
**
** HTMLReport - Provides a base class for creating reports in HTML.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: HTMLReport.cpp,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#include <stdio.h>
#include <stdlib.h>

#include <qprinter.h>
#include <qpainter.h>
#include <qsimplerichtext.h>
#include <qpaintdevicemetrics.h>
#include <qmime.h>
#include <qstylesheet.h>
#include <qregion.h>
#include <qpalette.h>

#include "HTMLReport.h"

#define Inherited HTMLReportData

HTMLReport::HTMLReport
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
    setCaption("Report");
	
    // Start out by setting our start and end dates.
	QDate       tmpDate = QDate::currentDate();
	myStartDate.setYMD(tmpDate.year(), tmpDate.month(), 1);
	myEndDate.setYMD(  tmpDate.year(), tmpDate.month(), tmpDate.daysInMonth());

    // Now, set the text on the buttons.
    char tmpStr[512];
    sprintf(tmpStr, "%s %02d, %04d", (const char *) myStartDate.monthName(myStartDate.month()), myStartDate.day(), myStartDate.year());
    // startDateButton->setText(tmpStr);
    
    sprintf(tmpStr, "%s %02d, %04d", (const char *) myEndDate.monthName(myEndDate.month()), myEndDate.day(), myEndDate.year());
    // endDateButton->setText(tmpStr);

    startDateCal->setDate(myStartDate);
    endDateCal->setDate(myEndDate);
    
    allowDates(REP_ALLDATES);
    allowGraph(0);
    
    // By default, we don't want to allow filters.
    allowFilters(0);
    
    // If we have a parent, don't show the close button or the title
    if (parent) {
        closeButton->hide();
    }

    userButton->hide();     // By default, we don't have a user button
    
    allowReproduction(1);   // We can graph/print/email by default.
}

HTMLReport::~HTMLReport()
{
}
void HTMLReport::graphReport()
{
}

void HTMLReport::printReportNew()
{
    QPrinter    qpr;
    QFont       fnt("helvetica");
    QString     context;
    QPainter    *p = new QPainter();
    QPalette    pal;

    qpr.setPageSize(QPrinter::Letter);

    if (qpr.setup()) {

        p->begin(&qpr);
        QPaintDeviceMetrics qpdm(p->device());
        QRect       rect;
        rect.setCoords(1,1,qpdm.width(), qpdm.height());
        QRegion     region(rect);

        QSimpleRichText *qrt = new QSimpleRichText(body->text(), 
                fnt, context, 
                QStyleSheet::defaultSheet(),
                QMimeSourceFactory::defaultFactory(), 
                qpdm.height());

        //qrt->draw(p, 0, 0, region, pal);
        
        p->end();

        /*

        // Create the command line....
        sprintf(cmdLine, "html2ps -n ");
        if (qpr.orientation() == QPrinter::Landscape) strcat(cmdLine, "-L ");
        if (qpr.colorMode() == QPrinter::GrayScale) strcat(cmdLine, "-g ");

        // Checking for file names must be last.
        if (qpr.outputToFile()) {
            strcat(cmdLine, "-o ");
            strcat(cmdLine, qpr.outputFileName());
        } else {
            strcat(cmdLine, "| lpr -P");
            strcat(cmdLine, (const char *) qpr.printerName());
        }
        FILE    *fp = popen(cmdLine, "w");
        fprintf(fp, "%s", (const char *) body->text());
        pclose(fp);
        */
    }
}

void HTMLReport::printReport()
{
    // We will use 'html2ps' to print reports now.
    char        cmdLine[4096];
    QPrinter    qpr;
    qpr.setPageSize(QPrinter::Letter);

    if (qpr.setup()) {

        // Create the command line....
        sprintf(cmdLine, "html2ps -n ");
        if (qpr.orientation() == QPrinter::Landscape) strcat(cmdLine, "-L ");
        if (qpr.colorMode() == QPrinter::GrayScale) strcat(cmdLine, "-g ");

        // Checking for file names must be last.
        if (qpr.outputToFile()) {
            strcat(cmdLine, "-o ");
            strcat(cmdLine, qpr.outputFileName());
        } else {
            strcat(cmdLine, "| lpr -P");
            strcat(cmdLine, (const char *) qpr.printerName());
        }
        FILE    *fp = popen(cmdLine, "w");
        fprintf(fp, "%s", (const char *) body->text());
        pclose(fp);
    }
}

void HTMLReport::emailReport()
{
}


void HTMLReport::closeReport()
{
    close();
}

void HTMLReport::editFilters()
{
}

void HTMLReport::refreshReport()
{
}

void HTMLReport::dateRangeSelected(const QString&)
{
}

void HTMLReport::userButtonClickedInt()
{
}


/*
** startDate  - Returns the current start date of the report.
*/

QDate HTMLReport::startDate()
{
    QDate   tmpDate = startDateCal->getQDate();
    myStartDate.setYMD(tmpDate.year(), tmpDate.month(), tmpDate.day());
    return myStartDate;
}

/*
** endDate  - Returns the current end date of the report.
*/

QDate HTMLReport::endDate()
{
    QDate   tmpDate = endDateCal->getQDate();
    myEndDate.setYMD(tmpDate.year(), tmpDate.month(), tmpDate.day());
    return myEndDate;
}

/*
** setStartDate - Sets the start date for the report.
*/

void HTMLReport::setStartDate(const QDate newDate)
{
    char    dateStr[1024];
    sprintf(dateStr, "%02d/%02d/%04d", newDate.month(), newDate.day(), newDate.year());
    startDateCal->setDate(dateStr);
    myStartDate = newDate;
}

/*
** setEndDate - Sets the end date for the report.
*/

void HTMLReport::setEndDate(const QDate newDate)
{
    myEndDate = newDate;
    char    dateStr[1024];
    sprintf(dateStr, "%02d/%02d/%04d", newDate.month(), newDate.day(), newDate.year());
    endDateCal->setDate(dateStr);
}


/*
** allowDates    - Toggles the dates on the window.  By turning it off, it
**                 prevents the user from seeing or showing the dates.
**                 Useful for "snapshot" reports like login counts.
*/

void HTMLReport::allowDates(int newSetting)
{
    myAllowDates = newSetting;
    // Start by hiding everything.
    startDateLabel->hide();
    startDateCal->hide();
    endDateLabel->hide();
    endDateCal->hide();
    generateButton->hide();
    dateList->hide();
    
    // If we have anything enabled, the generate button needs to be displayed.
    if (myAllowDates) generateButton->show();
    
    // Now, walk through the flags and enable anything that interests us.
    if (myAllowDates && REP_STARTDATE) {
        startDateLabel->show();
        startDateCal->show();
    }

    if (myAllowDates && REP_ENDDATE) {
        endDateLabel->show();
        endDateCal->show();
    }
    
    if (myAllowDates && REP_DATELIST) {
        dateList->show();
    }
}

/*
** allowFilters    - Toggles the filters button on the window.
*/

void HTMLReport::allowFilters(int newSetting)
{
    myAllowFilters = newSetting;
    if (myAllowFilters) {
        filterButton->show();
    } else {
        filterButton->hide();
    }
}


/*
** allowGraph   - Toggles the graph button on the window.
*/

void HTMLReport::allowGraph(int newSetting)
{
    myAllowGraph = newSetting;
    if (myAllowGraph) {
        graphButton->show();
    } else {
        graphButton->hide();
    }
}




/*
** allowReproduction - If set to false, the Graph, Print and Email buttons
**                     will be disabled.
*/

void HTMLReport::allowReproduction(int newSetting)
{
    if (newSetting) {
        printButton->show();
        emailButton->show();
    } else {
        printButton->hide();
        emailButton->hide();
    }
}

/*
** setUserButton - Sets the text of the user button and enables it.
*/

void HTMLReport::setUserButton(const char *newText)
{
    userButton->show();
    userButton->setText(newText);
}

