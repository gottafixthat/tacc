/*
** EmailTemplateSettings - Allows the user to pick which email templates
** get used for which processes.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvatel Corporation and R. Marc Lewis.
***************************************************************************
*/

#ifndef EMAILTEMPLATESETTINGS_H
#define EMAILTEMPLATESETTINGS_H

#include <qcombobox.h>
#include "TAAWidget.h"

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

