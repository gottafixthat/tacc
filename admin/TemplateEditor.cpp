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

#include <QtGui/QMessageBox>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>

#include <ADB.h>

#include <TemplateEditor.h>

using namespace Qt;

TemplateEditor::TemplateEditor(QWidget* parent, const char* name) : 
    TAAWidget(parent)
{
	setCaption( "Edit Email Templates" );
    
    // Create our widgets
    templateList = new Q3ListView(this, "templateList");
    templateList->addColumn("Template Name");
    templateList->setAllColumnsShowFocus(true);
    connect(templateList, SIGNAL(currentChanged(Q3ListViewItem *)), this, SLOT(templateSelected(Q3ListViewItem *)));

    QLabel *templateNameLabel = new QLabel(this, "templateNameLabel");
    templateNameLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    templateNameLabel->setText("Template Name:");

    templateName = new QLineEdit(this, "templateName");
    templateName->setMaxLength(60);
    connect(templateName, SIGNAL(textChanged(const QString &)), this, SLOT(textItemChanged(const QString &)));

    QLabel *subjectLabel = new QLabel(this, "subjectLabel");
    subjectLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    subjectLabel->setText("Default Subject:");

    subject = new QLineEdit(this, "subject");
    subject->setMaxLength(100);
    connect(subject, SIGNAL(textChanged(const QString &)), this, SLOT(textItemChanged(const QString &)));

    QLabel *editorLabel = new QLabel(this, "editorLabel");
    editorLabel->setAlignment(Qt::AlignTop|Qt::AlignRight);
    editorLabel->setText("Message Body:");

    editor = new Q3TextEdit(this, "editor");
    editor->setTextFormat(Qt::PlainText);
    editor->setWordWrap(Q3TextEdit::NoWrap);
    connect(editor, SIGNAL(textChanged()), this, SLOT(textItemChanged()));

    // Our action buttons
    QPushButton *newButton = new QPushButton(this, "newButton");
    newButton->setText("&New");
    connect(newButton, SIGNAL(clicked()), this, SLOT(newTemplate()));

    deleteButton = new QPushButton(this, "deleteButton");
    deleteButton->setText("&Delete");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteTemplate()));
    
    saveButton = new QPushButton(this, "saveButton");
    saveButton->setText("&Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveTemplate()));
    
    QPushButton *closeButton = new QPushButton(this, "closeButton");
    closeButton->setText("&Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    
    // Our layout.
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);

    Q3BoxLayout *wl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    wl->addWidget(templateList, 0);

    Q3GridLayout *gl = new Q3GridLayout(3,2);
    int curRow = 0;
    gl->addWidget(templateNameLabel,        curRow, 0);
    gl->addWidget(templateName,             curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addWidget(subjectLabel,             curRow, 0);
    gl->addWidget(subject,                  curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addWidget(editorLabel,              curRow, 0);
    gl->addWidget(editor,                   curRow, 1);
    gl->setRowStretch(curRow, 1);
    curRow++;
    gl->setColStretch(0,0);
    gl->setColStretch(1,1);

    wl->addLayout(gl, 1);

    ml->addLayout(wl, 1);

    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addWidget(newButton, 0);
    bl->addWidget(deleteButton, 0);
    bl->addStretch(1);
    bl->addWidget(saveButton, 0);
    bl->addWidget(closeButton, 0);

    ml->addLayout(bl, 0);


    itemChanged = false;
    saveButton->setEnabled(itemChanged);


    loadTemplates();


}


TemplateEditor::~TemplateEditor()
{
}


/**
 * refreshTemplate()
 *
 * Calls templateSelected which causes a refresh.
 */
void TemplateEditor::refreshTemplates(void)
{
    templateSelected(templateList->currentItem());
}

/**
 * loadTemplates()
 *
 * Loads the list of templates and fills the list with them.
 */
void TemplateEditor::loadTemplates()
{
    ADB     DB;

    templateList->clear();    
    DB.query("select TemplateID, Name, DefaultSubject, TextPart from EmailTemplates");
    if (DB.rowCount) while (DB.getrow()) {
        (void) new Q3ListViewItem(templateList, DB.curRow["Name"], DB.curRow["TemplateID"]);
    }
}


/*
** templateSelected - Calls each of the tabs so that they load the correct
**                    information when a login type is selected.
*/

void TemplateEditor::templateSelected(Q3ListViewItem *curItem)
{
    if (!curItem) return;

    if (itemChanged) {
        if (QMessageBox::warning(this, "Template Not Saved", "The selected template has not been saved.\nDo you wish to save it now?", "&Yes", "&No", 0, 1) == 0) {
            saveTemplate();
        }
    }

    ADBTable    ETDB;
    ETDB.setTableName("EmailTemplates");
    
    ETDB.get(curItem->key(1,0).toInt());

    myTemplateID = ETDB.getLong("TemplateID");
    origName     = ETDB.getStr("Name");
    origSubject  = ETDB.getStr("DefaultSubject");
    origBody     = ETDB.getStr("TextPart");

    templateName->setText(origName);
    subject->setText(origSubject);
    editor->setText(origBody);

    itemChanged = false;
    saveButton->setEnabled(false);

}

/*
** newTemplate - Inserts a new template entry in the database and makes
**               it the currently selected one.
*/

void TemplateEditor::newTemplate()
{
    Q3ListViewItem   *newItem;
    ADBTable    ETDB;
    ETDB.setTableName("EmailTemplates");
    
    ETDB.setValue("TemplateID",     (long) 0);
    ETDB.setValue("Name",           "New Template");
    ETDB.setValue("DefaultSubject", "");
    ETDB.setValue("TextPart",       "");
    ETDB.setValue("HTMLPart",       "");
    ETDB.ins();
    
    newItem = new Q3ListViewItem(templateList, ETDB.getStr("Name"), ETDB.getStr("TemplateID"));
    templateList->setSelected(newItem, TRUE);
    templateList->setCurrentItem(newItem);
    
}

/*
** saveTemplate - Saves the template we're editing into the database.
*/
void TemplateEditor::saveTemplate()
{
    Q3ListViewItem   *newItem;
    ADBTable    ETDB;
    ETDB.setTableName("EmailTemplates");
    
    ETDB.get(myTemplateID);
    ETDB.setValue("Name",           templateName->text().ascii());
    ETDB.setValue("DefaultSubject", subject->text().ascii());
    ETDB.setValue("TextPart",       editor->text().ascii());
    ETDB.setValue("HTMLPart",       "");
    ETDB.upd();
    
    itemChanged = false;
    saveButton->setEnabled(false);

    // Find this ID in the list and change the name.
    Q3ListViewItem *tmpItem;
    for (tmpItem = templateList->firstChild(); tmpItem; tmpItem = tmpItem->itemBelow()) {
        if (tmpItem->key(1,0).toInt() == myTemplateID) {
            tmpItem->setText(0, templateName->text());
        }
    }
}


/*
** deleteTemplate - Checks to see if the package is in use.  If not
**                 it prompts to verify that the user wishes to delete it.
**                 if it is in use, it tells the user that it cannot be
**                 deleted until it is no longer in use.
*/

void TemplateEditor::deleteTemplate()
{
    /*
    QListViewItem   *curItem = packageList->currentItem();
    // First, make sure that our current item is not null
    if (curItem == NULL) return;
    
    // Okay, now make sure that it is not in use.
    QApplication::setOverrideCursor(waitCursor);
    ADB     DB;
    long    myIntID = atol(curItem->key(2, 0));
    DB.query("select InternalID from Subscriptions where PackageNo = %ld", myIntID);
    QApplication::restoreOverrideCursor();
    
    if (DB.rowCount) {
        // This login type is in use.  Let the user know.
        QMessageBox::critical(this, "Delete Package", "The package is in use by customers, and may not be deleted.\n\nUpdate the customer subscriptions before proceeding.");
    } else {
        switch(QMessageBox::information(this, "Delete Package", 
          "Are you sure you wish to delete\nthe currently selected package?",
          "&No", "&Yes", "&Cancel", 0, 2)) {
          
            case 1:     // Delete it.
                QApplication::setOverrideCursor(waitCursor);
                DB.dbcmd("delete from Packages where InternalID = %ld", myIntID);
                DB.dbcmd("delete from PackageContents where PackageID = %ld", myIntID);
                DB.dbcmd("delete from PackagesData where PackageID = %ld", myIntID);
                
                // Refresh our list
                loadPackages();
                packageSelected(NULL);
                QApplication::restoreOverrideCursor();
                break;
                
            default:    // Don't delete it
                break;
        }
    }
    */
}


/*
** updateTemplate - A slot that gets called when the General tab emits
**                  packageChanged.  This way we can refresh the current
**                  item without reloading the list.
*/

void TemplateEditor::updateTemplate()
{
    /*
    QListViewItem   *curItem = packageList->currentItem();
    
    if (curItem != NULL) {
        char        tmpStr[1024];
		ADBTable    PDB;
		PDB.setTableName("Packages");

	    PDB.get(atol(curItem->key(2,0)));
	    
        curItem->setText(0, PDB.getStr("PackageTag"));
        if (PDB.getInt("Active")) {
            strcpy(tmpStr, "Y");
        } else {
            strcpy(tmpStr, "N");
        }
        curItem->setText(1, tmpStr);
    
    }
    */
}

/**
 * textItemChanged()
 *
 * Gets called whenever one of the text items has changed.
 * It toggles our itemChanged variable.
 */
void TemplateEditor::textItemChanged()
{
    // For speed's sake, we'll just set our itemChanged flag to true and enable the
    // save button.
    itemChanged = true;
    saveButton->setEnabled(true);
}

void TemplateEditor::textItemChanged(const QString &)
{
    textItemChanged();
}


// vim: expandtab
