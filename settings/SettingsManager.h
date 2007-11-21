/*
** Settings - Allows an admin to configure settings within TACC.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2007, R. Marc Lewis and Avvatel Corporation
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvatel Corporation and R. Marc Lewis.
***************************************************************************
*/

#ifndef Settings_included
#define Settings_included

#include <qlistbox.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qlabel.h>
#include "TAAWidget.h"
#include "GeneralSettings.h"
#include "BillingSettings.h"

class SettingsManager : public TAAWidget
{
    Q_OBJECT

public:
    SettingsManager(QWidget *parent = NULL, const char *name = NULL);
    virtual ~SettingsManager();

protected:
    QListBox        *sectionList;
    QWidgetStack    *sections;
    int             sectionCount;
    QLabel          *header;
    QPushButton     *saveButton;
    QPushButton     *cancelButton;

    GeneralSettings *general;
    BillingSettings *billing;

protected slots:
    void            saveSettings();
    void            cancelChanges();
    void            changeSection(int);
};

#endif // Settings_included

