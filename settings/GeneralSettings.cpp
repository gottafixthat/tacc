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

#include "GeneralSettings.h"

using namespace Qt;

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

    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 1, 1);

    Q3GridLayout *gl = new Q3GridLayout(2,2);
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


// vim: expandtab
