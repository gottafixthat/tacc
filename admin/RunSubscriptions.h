/**
 * RunSubscriptions.h - Class definition for the RunSubscriptions
 * widget.  RunSubscriptions brings up a window that gives a manager
 * the ability to run subscriptions for all customers that have
 * subscriptions pending.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef RunSubscriptions_included
#define RunSubscriptions_included

#include <qlabel.h>
#include <qpushbutton.h>
#include <q3listview.h>
#include <TAAWidget.h>

class RunSubscriptions : public TAAWidget
{
    Q_OBJECT

public:

    RunSubscriptions(QWidget* parent = NULL, const char* name = NULL);
    virtual ~RunSubscriptions();

protected slots:
    virtual void cancelAction();
    virtual void processSelections();

protected:
    Q3ListView   *subscrList;
    QPushButton *beginButton;
    QPushButton *cancelButton;
    QLabel      *totalSubscriptions;
    QLabel      *accountsReceivable;
    QLabel      *amountCharged;

private:
    void fillList(void);

    int     totSubscriptions;
    long    totActiveLogins;
    
    int     ARAcct;
    
    float   startingAR;
    float   totCharged;
};
#endif // RunSubscriptions_included
