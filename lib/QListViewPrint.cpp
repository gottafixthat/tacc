/**
 * QListViewPrint.cpp - A utility class that will "pretty print" any QListView
 * that is passed to it.
 *
 * Known Limitations - It won't indent children at this point.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <stdlib.h>
#include <math.h>

#include <QtGui/QApplication>
#include <Qt3Support/q3listview.h>
#include <QtCore/QDateTime>
#include <QtGui/QPrinter>
#include <QtGui/QPainter>
#include <Qt3Support/q3header.h>

#include <Cfg.h>
#include <QListViewPrint.h>

/**
 * QListViewPrint()
 *
 * Constructor.
 */
QListViewPrint::QListViewPrint(Q3ListView *srcList)
{
    myList = srcList;
    showDateLine = 0;
    myPrintSelectedOnly = 0;
}

/**
 * ~QListViewPrint()
 *
 * Destructor.
 */
QListViewPrint::~QListViewPrint()
{
}

/**
 * setTitle()
 *
 * Sets the title.
 */
void QListViewPrint::setTitle(const QString newTitle)
{
    myTitle = newTitle;
}

/**
 * setDateLine()
 *
 * Sets the Date Line.
 */
void QListViewPrint::setDateLine(const QString newDateLine)
{
    myDateLine = newDateLine;
    showDateLine = 1;

    // Printer offsets
    prLeftMargin  = 10;
    prRightMargin = 10;
}

/**
 * setPrintSelectedOnly()
 *
 * Tells the engine to print all or only selected items.
 */
void QListViewPrint::setPrintSelectedOnly(int selOnly)
{
    myPrintSelectedOnly = selOnly;
}


/**
 * print()
 *
 * Initiates the actual printing of the list.
 */
void QListViewPrint::print()
{
    QPrinter        prn(QPrinter::PrinterResolution);
    QPainter        *p;
    QBrush          bbrush;
    Q3ListViewItem   *curItem;
    
    int         pageNo = 1;
    int         totPages = 1;
    int         totLines = 0;
    
    prn.setPageSize(QPrinter::Letter);
    prn.setDocName("List Print");
    prn.setCreator("Total Accountability");
    
    // Initialize the printer device.
    if (!prn.setup()) return;
    // prn.setOutputFileName("/tmp/Report.ps");
    // prn.setOutputToFile(TRUE);
    QApplication::setOverrideCursor(Qt::waitCursor);
    
    p = new QPainter();
    p->begin(&prn);
    
    // Count the children.
    for (curItem = myList->firstChild(); curItem != 0; curItem = curItem->itemBelow()) {
        if (myPrintSelectedOnly) {
            if (curItem->isSelected()) totLines++;
        } else {
            totLines++;
        }
    }
    totPages = (totLines / 50) + 1;
    // fprintf(stderr, "The total number of pages is: %d\n", totPages);

    // Print the report...
    //QProgressDialog progress("Printing report...", "Abort", totLines, this);
    //progress.setTotalSteps(totPages);
    //progress.setProgress(0);
    curItem = myList->firstChild();
    for (pageNo = 1; pageNo < totPages + 1; pageNo++) {
        printHeader(p);
        curItem = printRows(p, curItem);
        printFooter(p, pageNo, totPages);
        if (pageNo < totPages) prn.newPage();
        //progress.setProgress(pageNo);
        //if (progress.wasCancelled()) pageNo = totPages + 1;
    }
    //progress.setProgress(totPages);
    
    // We're all done.  Close the printer device.
    p->end();
    delete(p);
    QApplication::restoreOverrideCursor();
}

void QListViewPrint::printHeader(QPainter *p)
{
    QDate       theDate;
    QTime       theTime;
    QRect       rect;
    QBrush      bbrush;
    QString     tmpSt;
    char        tmpstr[1024];
    int         xPos = prLeftMargin;
    
    float       headerPcts[20];
    float       colWidths[20];
    float       totWidth = 0;
    
    int         numCols;
    
    theDate = QDate::currentDate();
    theTime = QTime::currentTime();

    // p->rotate(55);
    // Draw our Company Name header and the date at the top of the page
    p->setFont(QFont("helvetica", 10, QFont::Bold));
    rect.setCoords(prLeftMargin, 30,  399, 60);
    p->drawText(rect, Qt::AlignLeft, cfgVal("CompanyName"));
    sprintf(tmpstr, "%s %s", (const char *) theDate.toString(), (const char *) theTime.toString());
    rect.setCoords(400, 30, prLeftMargin+540, 60);
    p->drawText(rect, Qt::AlignRight, tmpstr);
    
    // Now, draw the report title, centered on the page.
    // If we have a date range for the report, shrink down the title by 12
    // pixels so we can display the dates on the page.  If not, give the
    // title the full height.
    if (showDateLine) {
	    p->setFont(QFont("helvetica", 18, QFont::Bold));
	    rect.setCoords(0, 40, 611, 95);
	    p->drawText(rect, Qt::AlignCenter|Qt::AlignVCenter, myTitle);
	    p->setFont(QFont("helvetica",  8, QFont::Normal));
	    rect.setCoords(0, 96, 611, 112);
	    p->drawText(rect, Qt::AlignCenter|Qt::AlignVCenter, myDateLine);
    } else {
	    p->setFont(QFont("helvetica", 18, QFont::Bold));
	    rect.setCoords(0, 40, 611, 112);
	    p->drawText(rect, Qt::AlignCenter|Qt::AlignVCenter, myTitle);
    }

    // Now, calculate the column widths.
    // We'll take the strlens and convert them into pixel values
    // based on the percentage of the total.  That way, our report will
    // always be the full width of the page.
    numCols = myList->header()->count();
    
    if (!numCols) return;
    
    if (numCols > 20) numCols = 20;
    
    // Get the width of each header item.
    for (int i = 0; i < numCols; i++) {
        colWidths[i] = myList->header()->label(i).length();
    }

    // Now, get the longest item for each of the keys.
    Q3ListViewItem   *curItem;
    for (curItem = myList->firstChild(); curItem != NULL; curItem = curItem->itemBelow()) {
        for (int i = 0; i < numCols; i++) {
            if (curItem->key(i, 0).length() > colWidths[i]) {
                colWidths[i] = curItem->key(i, 0).length();
            }
        }
    }
    
    // Now, add up the widths.
    
    for (int i = 0; i < numCols; i++) {
        totWidth += colWidths[i];
        
    }
    
    // Now, get our percentages.
    for (int i = 0; i < numCols; i++) {
        headerPcts[i] = colWidths[i] / totWidth * 540;
        sprintf(tmpstr, "%f", rint(headerPcts[i]));
        prColWidths[i] = atoi(tmpstr);       // 1" margin
        // fprintf(stderr, "Column %d width = %f (%f) = %d pixels.\n", i, colWidths[i], headerPcts[i], prColWidths[i]);
    }
    
    // Now, draw the titles.
    //xPos = 36;
    xPos = prLeftMargin;
    p->setFont(QFont("helvetica", 8, QFont::Normal));
    for (int i = 0; i < numCols; i++) {
        rect.setCoords(xPos+1, 113, xPos + prColWidths[i] - 1, 129);
        bbrush.setStyle(Qt::SolidPattern);
        bbrush.setColor(Qt::black);
        //p->setBackgroundMode(OpaqueMode);
        p->setPen(Qt::white);
        p->fillRect(rect, bbrush);
        p->drawText(rect, Qt::AlignCenter|Qt::AlignVCenter, myList->header()->label(i));
        xPos += prColWidths[i];
    }
    // Reset our pen
    p->setBackgroundMode(Qt::TransparentMode);
    p->setPen(Qt::black);
}

void QListViewPrint::printFooter(QPainter *p, int PageNo, int totPages)
{
    QRect       rect;
    char        tmpstr[1024];
    
    // Now, draw the report title, centered on the page.
    p->setFont(QFont("helvetica", 8, QFont::Normal));
    rect.setCoords(0, 735, 611, 750);
    sprintf(tmpstr, "Page %d of %d", PageNo, totPages);
    p->drawText(rect, Qt::AlignCenter|Qt::AlignVCenter, tmpstr);

}

Q3ListViewItem *QListViewPrint::printRows(QPainter *p, Q3ListViewItem *startItem)
{
    QRect       rect;
    QBrush      bbrush;
    QString     tmpSt;
    int         yPos;
    int         xPos = prLeftMargin; 
    
    int         rowsPrinted = 0;
    
    int         numCols;
    
    // Now, draw the report title, centered on the page.
    p->setFont(QFont("helvetica", 8, QFont::Normal));

    numCols = myList->header()->count();
    if (!numCols) return(NULL);
    if (numCols > 20) numCols = 20;

    if (startItem != NULL) {
        int finished = 0;
        yPos     = 130;
        while(!finished) {
            if (!myPrintSelectedOnly || startItem->isSelected()) {
                xPos     = prLeftMargin;
                for (int i = 0; i < numCols; i++) {
                    rect.setCoords(xPos+1, yPos, xPos + prColWidths[i] - 1, yPos+11);
                    p->drawText(rect, myList->columnAlignment(i)|Qt::AlignVCenter, startItem->key(i,0));
                    xPos += prColWidths[i];
                }
                yPos += 12;
                if (++rowsPrinted >= 50) finished = 1;
            }
            startItem = startItem->itemBelow();
            if (startItem == NULL) finished = 1;
        }
    }
    
    return startItem;
}

