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

#include "DNSTemplatePicker.h"

#include <stdlib.h>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>
#include <ADB.h>

DNSTemplatePicker::DNSTemplatePicker(QWidget *parent, const char *name)
    : QDialog(parent, name, true)
{
    setCaption("Select DNS Template");
    // Create our widgets.
    templateList = new Q3ListView(this, "templateList");
    templateList->addColumn("Template Name");
    templateList->setAllColumnsShowFocus(true);
    connect(templateList, SIGNAL(selectionChanged(Q3ListViewItem *)), this, SLOT(itemSelected(Q3ListViewItem *)));
    connect(templateList, SIGNAL(doubleClicked(Q3ListViewItem *)), this, SLOT(itemDoubleClicked(Q3ListViewItem *)));


    selectButton = new QPushButton(this, "selectButton");
    selectButton->setText("&Select");
    connect(selectButton, SIGNAL(clicked()), this, SLOT(accept()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    // Create our layout
    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(templateList, 1);

    // Buttons.
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(selectButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    // All done with our layout.

    // Load the list of templates
    refreshList();

}

DNSTemplatePicker::~DNSTemplatePicker()
{
}

/*
** refreshList - Loads the list of DNS templates from the database and
**               fills the list with them.
*/

void DNSTemplatePicker::refreshList()
{
    ADB     db;
    db.query("select TemplateID, TemplateName from DNS_Templates where Active <> 0");
    if (db.rowCount) {
        while (db.getrow()) {
            (void) new Q3ListViewItem(templateList, db.curRow["TemplateName"], db.curRow["TemplateID"]);
        }
    }
}

/*
** itemSelected - Gets called when the user selects an item from the list
**                it updates our result with the TemplateID.
*/

void DNSTemplatePicker::itemSelected(Q3ListViewItem *curItem)
{
    if (curItem == NULL) {
        setResult(0);
        return;
    }

    setResult(atoi(curItem->key(1,0)));
}

/*
** itemDoubleClicked - Gets called when the user double clicks an item 
**                     from the list it updates our result with the 
**                     TemplateID then calls accept().
*/

void DNSTemplatePicker::itemDoubleClicked(Q3ListViewItem *curItem)
{
    if (curItem == NULL) {
        setResult(0);
        return;
    }

    setResult(atoi(curItem->key(1,0)));
    accept();
}

/*
** getTemplateID - Returns the template ID that the user picked.
*/

long DNSTemplatePicker::getTemplateID()
{
    if (templateList->currentItem() == NULL) {
        return 0;
    }

    return atoi(templateList->currentItem()->key(1,0));

}

// vim: expandtab

