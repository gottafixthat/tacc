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

#ifndef Settings_included
#define Settings_included

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <Qt3Support/q3widgetstack.h>
#include <Qt3Support/q3listbox.h>

#include <TAAWidget.h>
#include <AccountingSettings.h>
#include <GeneralSettings.h>
#include <BillingSettings.h>
#include <CompanyInfoSettings.h>
#include <EmailTemplateSettings.h>

class SettingsManager : public TAAWidget
{
    Q_OBJECT

public:
    SettingsManager(QWidget *parent = NULL, const char *name = NULL);
    virtual ~SettingsManager();

protected:
    Q3ListBox        *sectionList;
    Q3WidgetStack    *sections;
    int             sectionCount;
    QLabel          *header;
    QPushButton     *saveButton;
    QPushButton     *cancelButton;

    AccountingSettings  *accounting;
    GeneralSettings     *general;
    BillingSettings     *billing;
    CompanyInfoSettings *company;
    EmailTemplateSettings *templates;

protected slots:
    void            saveSettings();
    void            cancelChanges();
    void            changeSection(int);
};

#endif // Settings_included


// vim: expandtab
