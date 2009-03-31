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


#ifndef CCMonetra_included
#define CCMonetra_included

#include <mcve.h>

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtCore/QTimer>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

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
    virtual void custDoubleClicked(Q3ListViewItem *curItem);

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
    Q3ListView   *list;
    QPushButton *startButton;
    QPushButton *finishedButton;
    QPushButton *cancelButton;
    int         doReceipts;

};
#endif // CCMonetra_included

// vim: expandtab

