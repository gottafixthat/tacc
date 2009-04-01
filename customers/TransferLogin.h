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

#ifndef TRANSFERLOGIN_H
#define TRANSFERLOGIN_H

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3datetimeedit.h>

#include <TAAWidget.h>
#include <CustomerSearch.h>

class TransferLogin : public TAAWidget
{
    Q_OBJECT

public:

    TransferLogin(QWidget* parent = NULL, const char* name = NULL);
    virtual ~TransferLogin();

    void    setSourceLogin(long custID, const char * LoginID);

protected slots:
    virtual void doTransfer();
    virtual void cancelTransfer();

protected:
    QString         sourceLogin;
    long            myCustID;
    QLabel          *messageLabel;
    CustomerSearch  *custSearch;
    Q3DateEdit       *effectiveDate;

};
#endif

// vim: expandtab

