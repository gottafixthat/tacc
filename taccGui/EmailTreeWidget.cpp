/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <stdlib.h>
#include <math.h>

#include <QtGui/QApplication>
#include <QtCore/QDateTime>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

#include <Cfg.h>
#include <TAATools.h>
#include "EmailTreeWidget.h"

using namespace Qt;

/**
 * EmailTreeWidget()
 *
 * Constructor.
 */
EmailTreeWidget::EmailTreeWidget(QTreeWidget *srcTree, QWidget *parent) : 
    TAAWidget(parent, Qt::Window)
{
    myTree = srcTree;
    showDateLine = 0;
    myEmailSelectedOnly = false;

    setWindowTitle("Email Report");
    setAttribute(Qt::WA_DeleteOnClose);

    QLabel  *emailAddressLabel = new QLabel("Email Address:", this);
    emailAddressLabel->setAlignment(AlignVCenter|AlignRight);

    emailAddr = new QLineEdit(this);
    emailAddr->setMaxLength(80);
    QString tmpStr;
    tmpStr = curUser().userName;
    tmpStr += "@";
    tmpStr += cfgVal("EmailDomain");
    emailAddr->setText(tmpStr);

    includePlainText = new QCheckBox("Include plain-text report", this);
    includePlainText->setChecked(true);

    includeCSV = new QCheckBox("Attach report as CSV file", this);
    includeCSV->setChecked(false);

    QPushButton *sendButton = new QPushButton(this, "sendButton");
    sendButton->setText("&Send");
    sendButton->setDefault(true);
    connect(sendButton, SIGNAL(clicked()), this, SLOT(emailReport()));

    QPushButton *cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));


    QBoxLayout  *ml = new QBoxLayout(QBoxLayout::TopToBottom);

    QGridLayout *gl = new QGridLayout(2,3);
    int curRow = 0;
    gl->addWidget(emailAddressLabel,        curRow, 0);
    gl->addWidget(emailAddr,                curRow, 1);
    curRow++;

    gl->addWidget(includePlainText,         curRow, 1);
    curRow++;

    gl->addWidget(includeCSV,               curRow, 1);
    curRow++;
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);

    ml->addLayout(gl, 0);
    ml->addStretch(1);
    
    QBoxLayout  *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(sendButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl);
    setLayout(ml);
}

/**
 * ~EmailTreeWidget()
 *
 * Destructor.
 */
EmailTreeWidget::~EmailTreeWidget()
{
}

/**
 * setTitle()
 *
 * Sets the title.
 */
void EmailTreeWidget::setTitle(const QString newTitle)
{
    myTitle = newTitle;
}

/**
 * setDateLine()
 *
 * Sets the Date Line.
 */
void EmailTreeWidget::setDateLine(const QString newDateLine)
{
    myDateLine = newDateLine;
    showDateLine = 1;
}

/**
 * setEmailSelectedOnly()
 *
 * Tells the engine to print all or only selected items.
 */
void EmailTreeWidget::setEmailSelectedOnly(bool selOnly)
{
    myEmailSelectedOnly = selOnly;
}


/**
 * print()
 *
 * Initiates the actual printing of the list.
 */
void EmailTreeWidget::emailReport()
{
    int     colWidths[20];
    uint    numCols;
    QString fmtStr;
    char    tmpStr[4096];
    QString tmpQstr;
    QString colStr;
    QString txtBody;
    QString txtStr;
    QString csvName;
    QDate   tmpDate = QDate::currentDate();
    QTime   tmpTime = QTime::currentTime();
    QTreeWidgetItem *hdr;

    numCols = myTree->columnCount();
    
    if (!numCols) return;

    hdr = myTree->headerItem();

    //debug(5,"QListViewToCSV() returned %d lines\n", QListViewToCSV(myTree, "/tmp/test.csv"));

    // Get the width of each header item.
    for (uint i = 0; i < numCols; i++) {
        colWidths[i] = hdr->text(i).length();
    }

    // Now, get the longest item for each of the keys.
    QTreeWidgetItem *curItem;
    for (int j = 0; j < myTree->topLevelItemCount(); j++) {
        curItem = myTree->topLevelItem(j);
        for (uint i = 0; i < numCols; i++) {
            if (curItem->text(i).length() > colWidths[i]) {
                colWidths[i] = curItem->text(i).length();
            }
        }
    }
    
    // If dates are allowed, then put them in here.
    if (showDateLine) {
        txtBody += myDateLine;
        txtBody += "\n\n";
    }
    
    // Okay, now print our headers
    for (uint i = 0; i < numCols; i++) {
        colStr  = hdr->text(i);
        if (hdr->textAlignment(i) & Qt::AlignRight) {
            colStr = colStr.rightJustify(colWidths[i]+1);
        } else {
            colStr = colStr.leftJustify(colWidths[i]+1);
        }
        txtBody += colStr;
    }
    txtBody += "\n";

    // And now a line seperating the headers from the data.
    for (uint i = 0; i < numCols; i++) {
        strcpy(tmpStr, "");
        for (int n = 0; n < colWidths[i]; n++) strcat(tmpStr, "=");
        
        txtBody += tmpStr;
        txtBody += " ";
        
    }
    txtBody += "\n";
    
    // And now, finally, the data itself
    for (int j = 0; j < myTree->topLevelItemCount(); j++) {
        curItem = myTree->topLevelItem(j);
        for (uint i = 0; i < numCols; i++) {
            fmtStr = "%";
            if (!(curItem->textAlignment(i) & AlignRight)) fmtStr += "-";
            sprintf(tmpStr, "%d", colWidths[i]);
            fmtStr += tmpStr;
            fmtStr += "s ";
            txtBody += txtStr.sprintf(fmtStr.ascii(), curItem->text(i).ascii());
        }
        txtBody += "\n";
    }
    
    txtBody += txtStr.sprintf("\n\nReport generated on %s at %s\n\n", tmpDate.toString().ascii(), tmpTime.toString().ascii());

    // Now, create the email.
    QString     fromAddr;
    QString     subj;

    fromAddr = fromAddr.sprintf("%s@%s", curUser().userName, cfgVal("EmailDomain"));
    subj = subj.sprintf("Report - %s", myTitle.ascii());

    EmailMessage    msg;

    msg.setFrom(fromAddr);
    msg.setTo(emailAddr->text());
    msg.setSubject(subj);

    // Only allow them to not send the body if we're attaching a CSV
    if (includeCSV->isChecked()) {
        if (includePlainText->isChecked()) {
            msg.setBody(txtBody);
        }
    } else {
        msg.setBody(txtBody);
    }

    // Are we attaching the CSV part?
    if (includeCSV->isChecked()) {
        // Create a temp file name.
        csvName = makeTmpFileName("/tmp/report-XXXXXX.csv");
        QTreeWidgetToCSV(myTree, csvName.ascii());

        msg.addAttachment(csvName.ascii(), "text/csv", "report.csv");
    }

    msg.send();

    // Delete our csv file if necessary
    if (includeCSV->isChecked()) {
        unlink(csvName.ascii());
    }

    QMessageBox::information(this, "Email Report", "The report has been spooled for mailing.");
    close();
}



// vim: expandtab
