/*
** EmailTemplateSettings - Allows an admin to set the email templates used
** by various programs.
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
#include <qlabel.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3BoxLayout>
#include <TAATools.h>
#include <Cfg.h>
#include <ADB.h>
#include <EmailTemplateSettings.h>

/**
 * EmailTemplateSettings()
 *
 * Constructor.  Lays out the widget.
 */
EmailTemplateSettings::EmailTemplateSettings(QWidget *parent, const char *name) : 
    TAAWidget(parent, name)
{
    setCaption("Email Template Settings");

    QLabel *ccReceiptLabel = new QLabel(this, "ccReceiptLabel");
    ccReceiptLabel->setText("Credit Card Receipt:");
    ccReceiptLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    ccReceipt = new QComboBox(false, this, "ccReceipt");
    QToolTip::add(ccReceipt, "This is the template file that will be sent\nto customers after thier credit card has\nbeen successfully charged.");

    QLabel *ccDeclinedLabel = new QLabel(this, "ccDeclinedLabel");
    ccDeclinedLabel->setText("Credit Card Declined:");
    ccDeclinedLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    ccDeclined = new QComboBox(false, this, "ccDeclined");
    QToolTip::add(ccDeclined, "This is the template file that will be sent\nto customers after thier credit card has\nbeen declined.");

    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 1, 1);

    Q3GridLayout *gl = new Q3GridLayout(2,2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    int curRow = 0;

    gl->addWidget(ccReceiptLabel,   curRow, 0);
    gl->addWidget(ccReceipt,        curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addWidget(ccDeclinedLabel,  curRow, 0);
    gl->addWidget(ccDeclined,       curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    ml->addLayout(gl, 1);
    ml->addStretch(1);

    loadTemplates();
}

/**
 * ~EmailTemplateSettings()
 *
 * Destructor.
 */
EmailTemplateSettings::~EmailTemplateSettings()
{
}

/*
 * validateSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int EmailTemplateSettings::validateSettings()
{
    int retVal = 1;     // Assume success
    
    return retVal;
}

/*
 * saveSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int EmailTemplateSettings::saveSettings()
{
    int retVal = 1;     // Assume success

    updateCfgVal("CCReceiptTemplate", ccReceipt->currentText().ascii());
    updateCfgVal("CCDeclinedTemplate", ccDeclined->currentText().ascii());
    
    return retVal;
}

/**
 * loadTemplates()
 *
 * Loads the list of templates into the combo boxes and sets the
 * currently selected template name.
 */
void EmailTemplateSettings::loadTemplates()
{
    ADB     DB;
    QString tmpStr;
    int     itemNo = 0;

    DB.query("select Name from EmailTemplates order by Name");
    if (DB.rowCount) while (DB.getrow()) {
        tmpStr = DB.curRow["Name"];
        ccReceipt->insertItem(DB.curRow["Name"]);
        if (!tmpStr.compare(tmpStr, cfgVal("CCReceiptTemplate"))) ccReceipt->setCurrentItem(itemNo);

        ccDeclined->insertItem(DB.curRow["Name"]);
        if (!tmpStr.compare(tmpStr, cfgVal("CCDeclinedTemplate"))) ccDeclined->setCurrentItem(itemNo);
        itemNo++;
    }
}

