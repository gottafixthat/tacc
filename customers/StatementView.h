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

#ifndef STATEMENTVIEW_H
#define STATEMENTVIEW_H

#include <qlabel.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qlistview.h>

#include <TAAWidget.h>

class StatementView : public TAAWidget
{
    Q_OBJECT

public:

    StatementView(QWidget* parent = NULL, const char* name = NULL); 
    virtual ~StatementView();
    
    void    loadStatement(long stNo);
    
public slots:
    virtual void closeView();
    virtual void reEmailStatement();
    virtual void rePrintStatement();
    
protected:
    long myStNo;
    QLabel      *custName;
    QLabel      *custAddr1;
    QLabel      *custAddr2;
    QLabel      *custAddr3;
    QLabel      *custAddr4;

    QLabel      *statementNo;
    QLabel      *statementDate;
    QLabel      *chargesThrough;
    QLabel      *dueDate;
    QLabel      *terms;

    QLabel      *prevBalance;
    QLabel      *credits;
    QLabel      *newCharges;
    QLabel      *financeCharge;
    QLabel      *totalDue;

    QListView   *itemList;
    QPushButton *closeButton;
    QPushButton *printButton;
    QPushButton *emailButton;
    QLabel      *emailedTo;
    QLabel      *printedOn;

};
#endif

// vim: expandtab

