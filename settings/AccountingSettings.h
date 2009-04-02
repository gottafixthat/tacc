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

#ifndef ACCOUNTINGSETTINGS_H
#define ACCOUNTINGSETTINGS_H

#include <QtGui/QComboBox>

#include <TAAWidget.h>

class AccountingSettings : public TAAWidget
{
    Q_OBJECT

public:
    AccountingSettings(QWidget *parent = NULL, const char *name = NULL);
    virtual ~AccountingSettings();

    int             validateSettings();
    int             saveSettings();

protected:
    QComboBox       *acctsRecvAccount;
    QComboBox       *undepositedFundsAccount;
    QComboBox       *undepositedCCAccount;
    QComboBox       *collectionsAccount;
    int             *acctNoIDX;

};

#endif


// vim: expandtab
