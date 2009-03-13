/*
** GeneralSettings - General configuration options for TACC.  Stuff goes 
** here that won't fit in any more specific configuration option.
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
#include <TAATools.h>
#include "Cfg.h"
#include "GeneralSettings.h"

/*
 * GeneralSettings - Constructor.  Lays out the widget.
 */
GeneralSettings::GeneralSettings(QWidget *parent, const char *name) : TAAWidget(parent, name)
{
    setCaption("General Settings");

    QLabel *emailDomainLabel = new QLabel(this, "emailDomainLabel");
    emailDomainLabel->setText("Email Domain:");
    emailDomainLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    emailDomain = new QLineEdit(this, "emailDomain");
    emailDomain->setText(cfgVal("EmailDomain"));
    QToolTip::add(emailDomain, "This domain will be appended to outbound email\naddresses when no domain is specified.");

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 1, 1);

    QGridLayout *gl = new QGridLayout(2,2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    int curRow = 0;

    gl->addWidget(emailDomainLabel,     curRow, 0);
    gl->addWidget(emailDomain,          curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    ml->addLayout(gl, 1);

}

/*
 * ~GeneralSettings - Destructor.  Cleans up stuff.
 */
GeneralSettings::~GeneralSettings()
{
}

/*
 * validateSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int GeneralSettings::validateSettings()
{
    int retVal = 1;     // Assume success
    
    return retVal;
}

/*
 * saveSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int GeneralSettings::saveSettings()
{
    int retVal = 1;     // Assume success

    updateCfgVal("EmailDomain", (const char *)emailDomain->text());
    
    return retVal;
}


