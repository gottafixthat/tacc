/*
** $Id: CCMonetra.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** CCMonetra.h - Class to process credit cards through the Monetra payment
**               gateway (CCVS).
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2003, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: CCMonetra.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef CCMonetra_included
#define CCMonetra_included

#include <TAAWidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <mcve.h>

class CCMonetra : public TAAWidget
{
    Q_OBJECT

public:

    CCMonetra
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CCMonetra();

protected slots:
    virtual void cancelPressed();
    virtual void finishedPressed();
    virtual void processPressed();
    virtual void custDoubleClicked(QListViewItem *curItem);

private:
    virtual void fillList();

    long    totRows;
    long    dbRows;
    
    float   batchAmount;
    long    apprCount;
    float   apprAmount;
    long    declCount;
    float   declAmount;

signals:
    void    finished();

private slots:
    void    updateStatus();

protected:
    QLabel      *batchSize;
    QLabel      *amount;
    QLabel      *approvedCount;
    QLabel      *approvedTotal;
    QLabel      *declinedCount;
    QLabel      *declinedTotal;
    QListView   *list;
    QPushButton *startButton;
    QPushButton *finishedButton;
    QPushButton *cancelButton;
    int         doReceipts;

};
#endif // CCMonetra_included
