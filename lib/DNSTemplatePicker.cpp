/*
** $Id: DNSTemplatePicker.cpp,v 1.3 2004/01/02 23:56:14 marc Exp $
**
***************************************************************************
**
** DNSTemplatePicker - Allows the user to pick a DNS Template.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DNSTemplatePicker.cpp,v $
** Revision 1.3  2004/01/02 23:56:14  marc
** Domain Template Editor and SQL based DNS is (for the most part) fully functional and ready to use.
**
** Revision 1.2  2004/01/02 14:59:01  marc
** DNSTemplatePicker complete, added DNSUtils
**
** Revision 1.1  2004/01/01 01:57:54  marc
** *** empty log message ***
**
**
*/

#include "DNSTemplatePicker.h"

#include <stdlib.h>
#include <qlabel.h>
#include <qlayout.h>
#include <ADB.h>

DNSTemplatePicker::DNSTemplatePicker(QWidget *parent, const char *name)
    : QDialog(parent, name, true)
{
    setCaption("Select DNS Template");
    // Create our widgets.
    templateList = new QListView(this, "templateList");
    templateList->addColumn("Template Name");
    templateList->setAllColumnsShowFocus(true);
    connect(templateList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(itemSelected(QListViewItem *)));
    connect(templateList, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(itemDoubleClicked(QListViewItem *)));


    selectButton = new QPushButton(this, "selectButton");
    selectButton->setText("&Select");
    connect(selectButton, SIGNAL(clicked()), this, SLOT(accept()));

    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    // Create our layout
    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);
    ml->addWidget(templateList, 1);

    // Buttons.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
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
            (void) new QListViewItem(templateList, db.curRow["TemplateName"], db.curRow["TemplateID"]);
        }
    }
}

/*
** itemSelected - Gets called when the user selects an item from the list
**                it updates our result with the TemplateID.
*/

void DNSTemplatePicker::itemSelected(QListViewItem *curItem)
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

void DNSTemplatePicker::itemDoubleClicked(QListViewItem *curItem)
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
