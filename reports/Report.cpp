/*
** Report.cpp - A base class for generic reports.  Defines the bulk
**              of the window manipulation and allows generic printing,
**              emailing and graphing.
*/

#include "Report.h"

#include <qmessagebox.h>

#include <stdio.h>
#include <stdlib.h>
#include <qdatetm.h>
#include <qheader.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qrect.h>
#include <math.h>
#include <qlayout.h>

#include <calendar.h>
#include <qprogressdialog.h>
#include <qapplication.h>

#include <TAATools.h>
#include <Cfg.h>

Report::Report
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
	setCaption( "Report" );

    // Create our widgets and our layout
    startDateLabel = new QLabel(this);
    startDateLabel->setText("Start:");
    startDateLabel->setAlignment(AlignVCenter|AlignRight);

    startDateCal = new QDateEdit(QDate::currentDate(), this, "StartDate");
    
    endDateLabel = new QLabel(this);
    endDateLabel->setText("End:");
    endDateLabel->setAlignment(AlignVCenter|AlignRight);
    
    endDateCal = new QDateEdit(QDate::currentDate(), this, "StartDate");

    dateList = new QComboBox(false, this);
    dateList->insertItem("This Month");
    dateList->insertItem("Last Month");
    dateList->insertItem("This Year");
    dateList->insertItem("Last Year");
    dateList->insertItem("This Quarter");
    dateList->insertItem("Last Quarter");
    connect(dateList, SIGNAL(activated(const QString&)), this, SLOT(dateRangeSelected(const QString&)));


    generateButton = new QPushButton(this);
    generateButton->setText("&Generate");
    connect(generateButton, SIGNAL(clicked()), this, SLOT(refreshReport()));

    reportTitle = new QLabel(this);
    reportTitle->setText("reportTitle");
    reportTitle->setAlignment(AlignVCenter|AlignHCenter|ExpandTabs|WordBreak);

    list = new QListView(this);
    list->addColumn("First Column");
    connect(list, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(listItemSelected(QListViewItem *)));

    userText = new QTextView(this);
    userText->setFocusPolicy(QWidget::NoFocus);

    // The action buttons...
    filterButton = new QPushButton(this);
    filterButton->setText("&Filters");
    connect(filterButton, SIGNAL(clicked()), this, SLOT(editFilters()));

    userButton = new QPushButton(this);
    userButton->setText("User");
    connect(userButton, SIGNAL(clicked()), this, SLOT(userButtonClickedInt()));

    graphButton = new QPushButton(this);
    graphButton->setText("&Graph");
    connect(graphButton, SIGNAL(clicked()), this, SLOT(graphReport()));

    refreshButton = new QPushButton(this);
    refreshButton->setText("&Refresh");
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refreshReport()));

    printButton = new QPushButton(this);
    printButton->setText("&Print");
    connect(printButton, SIGNAL(clicked()), this, SLOT(printReport()));

    emailButton = new QPushButton(this);
    emailButton->setText("E&mail");
    connect(emailButton, SIGNAL(clicked()), this, SLOT(emailReport()));

    closeButton = new QPushButton(this);
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeReport()));


    // Now, do the layout for the widget.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    // A layout for the top row where our dates and such are
    dl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    dl->addWidget(startDateLabel,   0);
    dl->addWidget(startDateCal,     1);
    dl->addWidget(endDateLabel,     0);
    dl->addWidget(endDateCal,       1);
    dl->addStretch(1);
    dl->addWidget(dateList,         1);
    dl->addStretch(1);
    dl->addWidget(generateButton,   0);
    
    ml->addLayout(dl,           0);
    ml->addWidget(reportTitle,  0);
    ml->addWidget(list,         1);
    ml->addWidget(userText,     0);
    
    // Add the button layout
    bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(filterButton, 0);
    bl->addWidget(userButton,   0);
    bl->addWidget(graphButton,  0);
    bl->addWidget(refreshButton,0);
    bl->addWidget(printButton,  0);
    bl->addWidget(emailButton,  0);
    bl->addWidget(closeButton,  0);

    ml->addLayout(bl,           0);

	
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
        reportTitle->hide();
    }

    userText->hide();       // Hide the user text as well.

    userButton->hide();     // By default, we don't have a user button
    
    allowReproduction(1);   // We can graph/print/email by default.

    list->setAllColumnsShowFocus(true);
}


Report::~Report()
{
}

/*
** closeEvent  - Deletes the CallLog window when the user requests to
**               close the window.
*/

void Report::closeEvent(QCloseEvent *)
{
    // Close the window and delete everything we own...
    delete this;
}



/*
** setTitle - Sets the title of the report.
*/

void Report::setTitle(const char * newTitle)
{
    reportTitle->setText(newTitle);
}

/*
** startDate  - Returns the current start date of the report.
*/

QDate Report::startDate()
{
    QDate   tmpDate = startDateCal->date();
    myStartDate.setYMD(tmpDate.year(), tmpDate.month(), tmpDate.day());
    return myStartDate;
}

/*
** endDate  - Returns the current end date of the report.
*/

QDate Report::endDate()
{
    QDate   tmpDate = endDateCal->date();
    myEndDate.setYMD(tmpDate.year(), tmpDate.month(), tmpDate.day());
    return myEndDate;
}

/*
** setStartDate - Sets the start date for the report.
*/

void Report::setStartDate(const QDate newDate)
{
    char    dateStr[1024];
    sprintf(dateStr, "%02d/%02d/%04d", newDate.month(), newDate.day(), newDate.year());
    startDateCal->setDate(newDate);
    myStartDate = newDate;
}

/*
** setEndDate - Sets the end date for the report.
*/

void Report::setEndDate(const QDate newDate)
{
    myEndDate = newDate;
    char    dateStr[1024];
    sprintf(dateStr, "%02d/%02d/%04d", newDate.month(), newDate.day(), newDate.year());
    endDateCal->setDate(newDate);
}


/*
** allowDates    - Toggles the dates on the window.  By turning it off, it
**                 prevents the user from seeing or showing the dates.
**                 Useful for "snapshot" reports like login counts.
*/

void Report::allowDates(int newSetting)
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

void Report::allowFilters(int newSetting)
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

void Report::allowGraph(int newSetting)
{
    myAllowGraph = newSetting;
    if (myAllowGraph) {
        graphButton->show();
    } else {
        graphButton->hide();
    }
}



/*
** closeReport   - Closes the current report.
*/

void Report::closeReport()
{
    close();
}


/*
** graphReport   - Plots a generic graph of the data in the report in 
**                 a new window.
*/
void Report::graphReport()
{
#ifdef USEQWT
    Graph   *newGraph = new Graph(0, "Report Graph");
    plotGraph(newGraph->plotPtr());
    newGraph->show();
#endif
}

/*
** plotGraph     - A virtual function that must be overridden by the report
**                 if it wishes to plot a graph.
*/

#ifdef USEQWT
void Report::plotGraph(QwtPlot *plot)
{
    if (!plot) return;
}
#endif


/*
** printReport   - Creates a generic report and spits it out on the printer.
**
**                 This is a big, ugly, function, but it should produce 
**                 nice results.
*/

void Report::printReport()
{
    // QMessageBox::information(this, "Print Report", "Not yet implemented.");

    QPrinter        prn(QPrinter::PrinterResolution);
    QPainter        *p;
    QBrush          bbrush;
    QListViewItem   *curItem;
    
    int         pageNo = 1;
    int         totPages = 1;
    int         totLines = 0;
    
    prn.setPageSize(QPrinter::Letter);
    prn.setDocName("Standard Report");
    prn.setCreator("Total Accountability");

    
    // Initialize the printer device.
    if (!prn.setup()) return;
    // prn.setOutputFileName("/tmp/Report.ps");
    // prn.setOutputToFile(TRUE);
    QApplication::setOverrideCursor(waitCursor);
    
    p = new QPainter();
    p->begin(&prn);
    
    // Count the children.
    for (curItem = list->firstChild(); curItem != 0; curItem = curItem->itemBelow()) totLines++;
    totPages = (totLines / 50) + 1;
    // fprintf(stderr, "The total number of pages is: %d\n", totPages);

    // Print the report...
    QProgressDialog progress("Printing report...", "Abort", totLines, this);
    progress.setTotalSteps(totPages);
    progress.setProgress(0);
    curItem = list->firstChild();
    for (pageNo = 1; pageNo < totPages + 1; pageNo++) {
        printReportHeader(p);
        curItem = printReportRows(p, curItem);
        printReportFooter(p, pageNo, totPages);
        if (pageNo < totPages) prn.newPage();
        progress.setProgress(pageNo);
        if (progress.wasCancelled()) pageNo = totPages + 1;
    }
    progress.setProgress(totPages);
    
    // We're all done.  Close the printer device.
    p->end();
    delete(p);
    QApplication::restoreOverrideCursor();
}

void Report::printReportHeader(QPainter *p)
{
    QDate       theDate;
    QTime       theTime;
    QRect       rect;
    QBrush      bbrush;
    QString     tmpSt;
    char        tmpstr[1024];
    int         xPos = 36; 
    
    float       headerPcts[20];
    float       colWidths[20];
    float       totWidth = 0;
    
    int         numCols;
    
    theDate = QDate::currentDate();
    theTime = QTime::currentTime();

    // p->rotate(55);
    // Draw our Blarg header and the date at the top of the page
    p->setFont(QFont("helvetica", 10, QFont::Bold));
    rect.setCoords(36, 30,  399, 60);
    p->drawText(rect, AlignLeft, "Blarg! Online Services, Inc.");
    sprintf(tmpstr, "%s %s", (const char *) theDate.toString(), (const char *) theTime.toString());
    rect.setCoords(400, 30, 575, 60);
    p->drawText(rect, AlignRight, tmpstr);
    
    // Now, draw the report title, centered on the page.
    // If we have a date range for the report, shrink down the title by 12
    // pixels so we can display the dates on the page.  If not, give the
    // title the full height.
    if (myAllowDates) {
	    p->setFont(QFont("helvetica", 18, QFont::Bold));
	    rect.setCoords(0, 40, 611, 95);
	    p->drawText(rect, AlignCenter|AlignVCenter, reportTitle->text());
	    p->setFont(QFont("helvetica",  8, QFont::Normal));
	    rect.setCoords(0, 96, 611, 112);
	    sprintf(tmpstr, "For the period from %s through %s.",
	      (const char *) startDate().toString(),
          (const char *) endDate().toString()
        );
	    p->drawText(rect, AlignCenter|AlignVCenter, tmpstr);
    } else {
	    p->setFont(QFont("helvetica", 18, QFont::Bold));
	    rect.setCoords(0, 40, 611, 112);
	    p->drawText(rect, AlignCenter|AlignVCenter, reportTitle->text());
    }

    // Now, calculate the column widths.
    // We'll take the strlens and convert them into pixel values
    // based on the percentage of the total.  That way, our report will
    // always be the full width of the page.
    numCols = list->header()->count();
    
    if (!numCols) return;
    
    if (numCols > 20) numCols = 20;
    
    // Get the width of each header item.
    for (int i = 0; i < numCols; i++) {
        colWidths[i] = strlen(list->header()->label(i));
    }

    // Now, get the longest item for each of the keys.
    QListViewItem   *curItem;
    for (curItem = list->firstChild(); curItem != NULL; curItem = curItem->itemBelow()) {
        for (int i = 0; i < numCols; i++) {
            if ((int) strlen(curItem->key(i, 0)) > colWidths[i]) {
                colWidths[i] = strlen(curItem->key(i, 0));
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
    xPos = 36;
    p->setFont(QFont("helvetica", 8, QFont::Normal));
    for (int i = 0; i < numCols; i++) {
        rect.setCoords(xPos+1, 113, xPos + prColWidths[i] - 1, 129);
        bbrush.setStyle(SolidPattern);
        bbrush.setColor(black);
        p->setBackgroundMode(OpaqueMode);
        p->setPen(white);
        p->fillRect(rect, bbrush);
        p->drawText(rect, AlignCenter|AlignVCenter, list->header()->label(i));
        xPos += prColWidths[i];
    }
    // Reset our pen
    p->setBackgroundMode(TransparentMode);
    p->setPen(black);
}

void Report::printReportFooter(QPainter *p, int PageNo, int totPages)
{
    QRect       rect;
    char        tmpstr[1024];
    
    // Now, draw the report title, centered on the page.
    p->setFont(QFont("helvetica", 8, QFont::Normal));
    rect.setCoords(0, 735, 611, 750);
    sprintf(tmpstr, "Page %d of %d", PageNo, totPages);
    p->drawText(rect, AlignCenter|AlignVCenter, tmpstr);

}

QListViewItem *Report::printReportRows(QPainter *p, QListViewItem *startItem)
{
    QRect       rect;
    QBrush      bbrush;
    QString     tmpSt;
    int         yPos;
    int         xPos = 36; 
    
    int         rowsPrinted = 0;
    
    int         numCols;
    
    // Now, draw the report title, centered on the page.
    p->setFont(QFont("helvetica", 8, QFont::Normal));

    numCols = list->header()->count();
    if (!numCols) return(NULL);
    if (numCols > 20) numCols = 20;

    if (startItem != NULL) {
        int finished = 0;
        yPos     = 130;
        while(!finished) {
	        xPos     = 36;
            for (int i = 0; i < numCols; i++) {
		        rect.setCoords(xPos+1, yPos, xPos + prColWidths[i] - 1, yPos+11);
				p->drawText(rect, list->columnAlignment(i)|AlignVCenter, startItem->key(i,0));
		        xPos += prColWidths[i];
		    }
		    yPos += 12;
            startItem = startItem->itemBelow();
            if (startItem == NULL) finished = 1;
            if (++rowsPrinted >= 50) finished = 1;
        }
    }
    
    return startItem;
}



/*
** emailReport   - Creates a generic report and emails it to the current user.
*/

void Report::emailReport()
{
    int     colWidths[20];
    int     numCols;
    char    fmtStr[4096];
    char    tmpStr[4096];
    QString tmpQstr;
    QDate   tmpDate = QDate::currentDate();
    QTime   tmpTime = QTime::currentTime();
    FILE    *fp;
    char    fname[2048];

    numCols = list->header()->count();
    
    if (!numCols) return;
    
    // Get the width of each header item.
    for (int i = 0; i < numCols; i++) {
        colWidths[i] = strlen(list->header()->label(i));
    }

    // Now, get the longest item for each of the keys.
    QListViewItem   *curItem;
    for (curItem = list->firstChild(); curItem != NULL; curItem = curItem->itemBelow()) {
        for (int i = 0; i < numCols; i++) {
            if ((int) strlen(curItem->key(i, 0)) > colWidths[i]) {
                colWidths[i] = strlen(curItem->key(i, 0));
            }
        }
    }
    
    // Open the file to save the message to for spooling.
    sprintf(fname, "/var/spool/taamail/Report-%s-%02d%02d%02d%02d",
      curUser().userName,
      tmpTime.hour(),
      tmpTime.minute(),
      tmpTime.second(),
      tmpTime.msec()
    );
    
    fp = fopen(fname, "w");
    if (fp) {
        // The message header.
        fprintf(fp, "From: %s@%s\n", curUser().userName, cfgVal("EmailDomain"));
        fprintf(fp, "To: %s@%s\n", curUser().userName, cfgVal("EmailDomain"));
        fprintf(fp, "Subject: Report - %s\n", (const char *) reportTitle->text());
        fprintf(fp, "\n\n");
    
    
        // If dates are allowed, then put them in here.
        if (myAllowDates) {
            fprintf(fp, "%11s: %s\n", "Start Date", (const char *) startDate().toString());
            fprintf(fp, "%11s: %s\n", "End Date", (const char *) endDate().toString());
            fprintf(fp, "\n\n");
        }
	    
	    // Okay, now print our headers
	    for (int i = 0; i < numCols; i++) {
	        strcpy(fmtStr, "%");
	        if (list->columnAlignment(i) != AlignRight) strcat(fmtStr, "-");
	        sprintf(tmpStr, "%d", colWidths[i]);
	        strcat(fmtStr, tmpStr);
	        strcat(fmtStr, "s ");
	        fprintf(fp, fmtStr, (const char *) list->header()->label(i));
	    }
	    fprintf(fp, "\n");

	    // And now a line seperating the headers from the data.
	    for (int i = 0; i < numCols; i++) {
	        strcpy(fmtStr, "%s ");
	        strcpy(tmpStr, "");
	        for (int n = 0; n < colWidths[i]; n++) strcat(tmpStr, "=");
	        
	        fprintf(fp, fmtStr, tmpStr);
	        
	    }
	    fprintf(fp, "\n");
	    
	    // And now, finally, the data itself
	    for (curItem = list->firstChild(); curItem != NULL; curItem = curItem->itemBelow()) {
		    for (int i = 0; i < numCols; i++) {
		        strcpy(fmtStr, "%");
		        if (list->columnAlignment(i) != AlignRight) strcat(fmtStr, "-");
		        sprintf(tmpStr, "%d", colWidths[i]);
		        strcat(fmtStr, tmpStr);
		        strcat(fmtStr, "s ");
		        fprintf(fp, fmtStr, (const char *) curItem->key(i, 0));
		    }
		    fprintf(fp, "\n");
	    }
	    
	    fprintf(fp, "\n\n");
	    fprintf(fp, "Report generated on %s at %s\n\n", (const char *)tmpDate.toString(), (const char *) tmpTime.toString());
	    fclose(fp);

        QMessageBox::information(this, "Email Report", "The report has been spooled for mailing.");

    } else {
        QMessageBox::critical(this, "Email Report", "Unable to spool report for mailing.");
    }

}

/*
** dateRangeSelected  - Changes the dates in the date spinboxes when the
**                      user selects a text description of a date range.
*/

void Report::dateRangeSelected(const char *newRange)
{
    QDate   today = QDate::currentDate();
    
    if (!strcmp("This Month", newRange)) {
        myStartDate.setYMD(today.year(), today.month(), 1);
        myEndDate.setYMD(today.year(), today.month(), today.daysInMonth());
    }
    if (!strcmp("Last Month", newRange)) {
        if (today.month() == 1) {
            today.setYMD(today.year()-1, 12, 1);
        } else {
            today.setYMD(today.year(), today.month()-1, 1);
        }
        myStartDate.setYMD(today.year(), today.month(), 1);
        myEndDate.setYMD(today.year(), today.month(), today.daysInMonth());
    }
    if (!strcmp("This Year", newRange)) {
        myStartDate.setYMD(today.year(), 1, 1);
        myEndDate.setYMD(today.year(), 12, 31);
    }
    if (!strcmp("Last Year", newRange)) {
        myStartDate.setYMD(today.year()-1, 1, 1);
        myEndDate.setYMD(today.year()-1, 12, 31);
    }
    if (!strcmp("This Quarter", newRange)) {
        // This one is trickier.  We have to figure out what month we are
        // in and then set the date accordingly.
        int thisMonth = today.month();
        switch (thisMonth) {
            case 1:         // First quarter.
            case 2:
            case 3:
                myStartDate.setYMD(today.year(), 1, 1);
                myEndDate.setYMD(today.year(), 3, 31);
                break;
                
            case 4:         // Second quarter.
            case 5:
            case 6:
                myStartDate.setYMD(today.year(), 4, 1);
                myEndDate.setYMD(today.year(), 6, 30);
                break;
                
            case 7:         // Third quarter.
            case 8:
            case 9:
                myStartDate.setYMD(today.year(), 7, 1);
                myEndDate.setYMD(today.year(), 9, 30);
                break;
                
            default:        // Last Quarter
                myStartDate.setYMD(today.year(), 10, 1);
                myEndDate.setYMD(today.year(), 12, 31);
                break;
                
        }
    }
    if (!strcmp("Last Quarter", newRange)) {
        // This one is trickier still.  We have to figure out what month 
        // quarter we are in, and then subtract three months from it.
        // in and then set the date accordingly.
        int thisMonth = today.month();

        switch (thisMonth) {
            case 1:         // Last quarter of last year.
            case 2:
            case 3:
                myStartDate.setYMD(today.year()-1, 10, 1);
                myEndDate.setYMD(today.year()-1, 12, 31);
                break;
                
            case 4:         // first quarter of this year.
            case 5:
            case 6:
                myStartDate.setYMD(today.year(), 1, 1);
                myEndDate.setYMD(today.year(), 3, 31);
                break;

            case 7:         // Second quarter.
            case 8:
            case 9:
                myStartDate.setYMD(today.year(), 4, 1);
                myEndDate.setYMD(today.year(), 6, 30);
                break;
                
            default:        // Third quarter of this year.
                myStartDate.setYMD(today.year(), 7, 1);
                myEndDate.setYMD(today.year(), 9, 30);
                break;
        }
    }
    
    char    dateStr[1024];
    sprintf(dateStr, "%02d/%02d/%04d", myStartDate.month(), myStartDate.day(), myStartDate.year());
    startDateCal->setDate(myStartDate);
    sprintf(dateStr, "%02d/%02d/%04d", myEndDate.month(), myEndDate.day(), myEndDate.year());
    endDateCal->setDate(myEndDate);
    refreshReport();
}

void Report::dateRangeSelected(const QString &newRange)
{
    dateRangeSelected((const char *)newRange);
}

/*
** editStartDate   - Gets a new start date from the user.
*/

void Report::editStartDate()
{
}

/*
** editEndDate     - Gets a new end date from the user.
*/

void Report::editEndDate()
{
}


void Report::userButtonClickedInt()
{
    emit userButtonClicked();
}


/*
** allowReproduction - If set to false, the Graph, Print and Email buttons
**                     will be disabled.
*/

void Report::allowReproduction(int newSetting)
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

void Report::setUserButton(const char *newText)
{
    userButton->show();
    userButton->setText(newText);
}

