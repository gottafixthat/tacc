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


#ifndef CCBatch_included
#define CCBatch_included

#include <TAAWidget.h>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <Qt3Support/q3listview.h>
#include <QtGui/QPushButton>

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
    virtual void custDoubleClicked(Q3ListViewItem *curItem);

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
    Q3ListView   *list;
    QPushButton *exportButton;
    QPushButton *finishedButton;
    QPushButton *cancelButton;

};
#endif // CCBatch_included

// vim: expandtab

