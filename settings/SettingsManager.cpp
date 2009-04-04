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

#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <Qt3Support/Q3BoxLayout>
#include <Qt3Support/Q3Frame>

#include "SettingsManager.h"

using namespace Qt;

/*
 * SettingsManager - Constructor.  Lays out the widget.
 */
SettingsManager::SettingsManager(QWidget *parent, const char *name) : TAAWidget(parent)
{
    setCaption("Settings Manager");

    sectionCount = 0;
    sectionList = new Q3ListBox(this, "sectionList");
    connect(sectionList, SIGNAL(selected(int)), this, SLOT(changeSection(int)));
    connect(sectionList, SIGNAL(highlighted(int)), this, SLOT(changeSection(int)));

    header = new QLabel(this, "header");
    header->setText("Settings");
    header->setFrameStyle(Q3Frame::Panel | Q3Frame::Sunken);
    header->setAlignment(AlignVCenter | AlignLeft);
    header->setPaletteBackgroundColor(Qt::blue);
    header->setPaletteForegroundColor(Qt::white);
    QFont   tmpFont = header->font();
    tmpFont.setBold(true);
    tmpFont.setPointSize(tmpFont.pointSize()+1);
    header->setFont(tmpFont);


    sections = new Q3WidgetStack(this, "sections");

    general = new GeneralSettings(this, "General");
    sectionList->insertItem("General");
    sections->addWidget(general, sectionCount);
    sectionCount++;

    billing = new BillingSettings(this, "Billing");
    sectionList->insertItem("Billing");
    sections->addWidget(billing, sectionCount);
    sectionCount++;

    accounting = new AccountingSettings(this, "Accounting");
    sectionList->insertItem("Accounting");
    sections->addWidget(accounting, sectionCount);
    sectionCount++;

    company = new CompanyInfoSettings(this, "Company Info");
    sectionList->insertItem("Company Info");
    sections->addWidget(company, sectionCount);
    sectionCount++;

    templates = new EmailTemplateSettings(this, "Email Templates");
    sectionList->insertItem("Email Templates");
    sections->addWidget(templates, sectionCount);
    sectionCount++;


    HorizLine   *hLine = new HorizLine(this, "hLine");
    VertLine    *vLine = new VertLine(this, "vLine");

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelChanges()));

    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::LeftToRight, 0, 0);
    ml->addWidget(sectionList, 0);

    Q3BoxLayout *sl = new Q3BoxLayout(Q3BoxLayout::TopToBottom, 0);
    sl->addWidget(header, 0);
    sl->addWidget(sections, 1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);
    sl->addWidget(hLine, 0);
    sl->addLayout(bl, 0);
    ml->addWidget(vLine, 0);
    ml->addLayout(sl, 1);

    // Highlight the first entry in the list.
    sectionList->setSelected(0, true);
}

/*
 * ~SettingsManager - Destructor.  Cleans up stuff.
 */
SettingsManager::~SettingsManager()
{
}

/*
 * saveSettings - Slot that gets called when the user clicks
 * the Save button.
 */
void SettingsManager::saveSettings()
{
    // Call each of the widgets one by one to validate their settings
    if (!accounting->validateSettings()) {
        sections->raiseWidget(accounting);
        sectionList->setSelected(sectionList->findItem(accounting->name(), Q3ListBox::ExactMatch), true);
        return;
    }

    if (!general->validateSettings()) {
        sections->raiseWidget(general);
        sectionList->setSelected(sectionList->findItem(general->name(), Q3ListBox::ExactMatch), true);
        return;
    }

    if (!billing->validateSettings()) {
        sections->raiseWidget(billing);
        sectionList->setSelected(sectionList->findItem(billing->name(), Q3ListBox::ExactMatch), true);
        return;
    }

    if (!company->validateSettings()) {
        sections->raiseWidget(company);
        sectionList->setSelected(sectionList->findItem(company->name(), Q3ListBox::ExactMatch), true);
        return;
    }

    if (!templates->validateSettings()) {
        sections->raiseWidget(templates);
        sectionList->setSelected(sectionList->findItem(templates->name(), Q3ListBox::ExactMatch), true);
        return;
    }



    // Now call each of the widgets one by one to save their settings
    if (!accounting->saveSettings()) {
        sections->raiseWidget(accounting);
        sectionList->setSelected(sectionList->findItem(accounting->name(), Q3ListBox::ExactMatch), true);
        return;
    }

    if (!general->saveSettings()) {
        sections->raiseWidget(general);
        sectionList->setSelected(sectionList->findItem(general->name(), Q3ListBox::ExactMatch), true);
        return;
    }

    if (!billing->saveSettings()) {
        sections->raiseWidget(billing);
        sectionList->setSelected(sectionList->findItem(billing->name(), Q3ListBox::ExactMatch), true);
        return;
    }

    if (!company->saveSettings()) {
        sections->raiseWidget(company);
        sectionList->setSelected(sectionList->findItem(company->name(), Q3ListBox::ExactMatch), true);
        return;
    }

    if (!templates->saveSettings()) {
        sections->raiseWidget(templates);
        sectionList->setSelected(sectionList->findItem(templates->name(), Q3ListBox::ExactMatch), true);
        return;
    }

    // Close down the window
    delete this;
}

/*
 * cancelChanges - Slot that gets called when the user clicks
 * the Cancel button.
 */
void SettingsManager::cancelChanges()
{
    // Close down the window
    delete this;
}

/*
 * changeSection - Connected to the selectionList, when the selection
 * is changed, this function gets called.  It raises the widget in the 
 * widget stack that is associated with the selected item.
 */
void SettingsManager::changeSection(int item)
{
    QString capStr;
    sections->raiseWidget(item);
    capStr = sections->visibleWidget()->name();
    capStr += " Settings";
    header->setText(capStr);
}




// vim: expandtab
