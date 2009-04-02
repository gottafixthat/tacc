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
#include <QtGui/QToolTip>
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3BoxLayout>

#include <TAATools.h>
#include <Cfg.h>

#include "CompanyInfoSettings.h"

using namespace Qt;

/**
 * CompanyInfoSettings()
 *
 * Constructor.
 */
CompanyInfoSettings::CompanyInfoSettings(QWidget *parent, const char *name) : TAAWidget(parent, name)
{
    setCaption("Company Information");

    QLabel *companyNameLabel = new QLabel(this, "companyNameLabel");
    companyNameLabel->setText("Company Name:");
    companyNameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    companyName = new QLineEdit(this, "companyName");
    companyName->setText(cfgVal("CompanyName"));
    QToolTip::add(companyName, "Enter the company name here.  This will appear on\nvarious headers including reports, statements, etc.");

    QLabel *addressLine1Label = new QLabel(this, "addressLine1Label");
    addressLine1Label->setText("Address 1:");
    addressLine1Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    addressLine1 = new QLineEdit(this, "addressLine1");
    addressLine1->setText(cfgVal("CompanyAddr1"));
    QToolTip::add(addressLine1, "The address lines.  These three lines will print below\nthe company name in order and without additional formatting\non things such as statements, etc.");

    QLabel *addressLine2Label = new QLabel(this, "addressLine2Label");
    addressLine2Label->setText("Address 2:");
    addressLine2Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    addressLine2 = new QLineEdit(this, "addressLine2");
    addressLine2->setText(cfgVal("CompanyAddr2"));

    QLabel *addressLine3Label = new QLabel(this, "addressLine3Label");
    addressLine3Label->setText("Address 3:");
    addressLine3Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    addressLine3 = new QLineEdit(this, "addressLine3");
    addressLine3->setText(cfgVal("CompanyAddr3"));

    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 1, 1);

    Q3GridLayout *gl = new Q3GridLayout(2,2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    int curRow = 0;

    gl->addWidget(companyNameLabel,     curRow, 0);
    gl->addWidget(companyName,          curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;
    gl->addWidget(addressLine1Label,    curRow, 0);
    gl->addWidget(addressLine1,         curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;
    gl->addWidget(addressLine2Label,    curRow, 0);
    gl->addWidget(addressLine2,         curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;
    gl->addWidget(addressLine3Label,    curRow, 0);
    gl->addWidget(addressLine3,         curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    ml->addLayout(gl, 0);
    ml->addStretch(1);

}

/**
 * ~CompanyInfoSettings()
 *
 * Destructor.
 */
CompanyInfoSettings::~CompanyInfoSettings()
{
}

/**
 * validateSettings()
 *
 * Slot that gets called when the user clicks the save button.
 */
int CompanyInfoSettings::validateSettings()
{
    int retVal = 1;     // Assume success
    
    return retVal;
}

/**
 * saveSettings()
 *
 * Slot that gets called when the user clicks the save button.
 */
int CompanyInfoSettings::saveSettings()
{
    int retVal = 1;     // Assume success


    updateCfgVal("CompanyName", companyName->text().ascii());
    updateCfgVal("CompanyAddr1", addressLine1->text().ascii());
    updateCfgVal("CompanyAddr2", addressLine2->text().ascii());
    updateCfgVal("CompanyAddr3", addressLine3->text().ascii());
    
    return retVal;
}



// vim: expandtab
