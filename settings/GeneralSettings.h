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

#ifndef GeneralSettings_included
#define GeneralSettings_included

#include <QtGui/QLineEdit>

#include <TAAWidget.h>

class GeneralSettings : public TAAWidget
{
    Q_OBJECT

public:
    GeneralSettings(QWidget *parent = NULL, const char *name = NULL);
    virtual ~GeneralSettings();

    int             validateSettings();
    int             saveSettings();

protected:
    QLineEdit       *emailDomain;

};

#endif // GeneralSettings_included


// vim: expandtab
