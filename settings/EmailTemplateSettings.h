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

#ifndef EMAILTEMPLATESETTINGS_H
#define EMAILTEMPLATESETTINGS_H

#include <QtGui/QComboBox>

#include <TAAWidget.h>

class EmailTemplateSettings : public TAAWidget
{
    Q_OBJECT

public:
    EmailTemplateSettings(QWidget *parent = NULL, const char *name = NULL);
    virtual ~EmailTemplateSettings();

    int             validateSettings();
    int             saveSettings();

protected:
    QComboBox       *ccReceipt;
    QComboBox       *ccDeclined;
    void            loadTemplates();

};

#endif


// vim: expandtab
