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

#include <qlayout.h>
#include "SettingsManager.h"

/*
 * SettingsManager - Constructor.  Lays out the widget.
 */
SettingsManager::SettingsManager(QWidget *parent, const char *name) : TAAWidget(parent, name)
{
    setCaption("Settings Manager");

    sectionList = new QListBox(this, "sectionList");
    connect(sectionList, SIGNAL(selected(int)), this, SLOT(changeSection(int)));
    connect(sectionList, SIGNAL(highlighted(int)), this, SLOT(changeSection(int)));

    sections = new QWidgetStack(this, "sections");

    general = new GeneralSettings(this, "generalSettings");
    sectionList->insertItem("General");
    sections->addWidget(general, 0);

    HorizLine   *hLine = new HorizLine(this, "hLine");
    VertLine    *vLine = new VertLine(this, "vLine");

    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelChanges()));

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::LeftToRight, 0, 0);
    ml->addWidget(sectionList, 0);

    QBoxLayout *sl = new QBoxLayout(QBoxLayout::TopToBottom, 0);
    sl->addWidget(sections, 1);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(cancelButton, 0);
    sl->addWidget(hLine, 0);
    sl->addLayout(bl, 0);
    ml->addWidget(vLine, 0);
    ml->addLayout(sl, 1);

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
    // Call each of the widgets one by one to save their settings

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
    sections->raiseWidget(item);
}



