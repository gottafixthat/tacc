/*
** $Id: CCBatch.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** CCBatch.h - Class definitions for the CCBatch functions that allow us
**             to process a batch of credit cards.
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
** $Log: CCBatch.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef CCBatch_included
#define CCBatch_included

#include <TAAWidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>

class CCBatch : public TAAWidget
{
    Q_OBJECT

public:

    CCBatch
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CCBatch();

protected slots:
    virtual void cancelPressed();
    virtual void finishedPressed();
    virtual void exportPressed();
    virtual void custDoubleClicked(QListViewItem *curItem);

private:
    virtual void fillList();

    long    totRows;
    long    dbRows;
    
    float   batchAmount;
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
    QPushButton *exportButton;
    QPushButton *finishedButton;
    QPushButton *cancelButton;

};
#endif // CCBatch_included
