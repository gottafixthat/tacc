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

#ifndef COMPANYINFOSETTINGS_H
#define COMPANYINFOSETTINGS_H

#include <QtGui/QLineEdit>

#include <TAAWidget.h>

class CompanyInfoSettings : public TAAWidget
{
    Q_OBJECT

public:
    CompanyInfoSettings(QWidget *parent = NULL, const char *name = NULL);
    virtual ~CompanyInfoSettings();

    int             validateSettings();
    int             saveSettings();

protected:
    QLineEdit       *companyName;
    QLineEdit       *addressLine1;
    QLineEdit       *addressLine2;
    QLineEdit       *addressLine3;

};

#endif


// vim: expandtab
