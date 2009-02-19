/**
 * TelephonySettings.cpp - Configuration options for telephony settings.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <qlayout.h>
#include <qlabel.h>
#include <qtooltip.h>
#include "TAAConfig.h"
#include "Cfg.h"
#include "TelephonySettings.h"

/*
 * TelephonySettings - Constructor.  Lays out the widget.
 */
TelephonySettings::TelephonySettings(QWidget *parent, const char *name) : TAAWidget(parent, name)
{
    setCaption("Telephony Settings");

    asteriskManager = new QCheckBox("Enable Asterisk Manager", this, "asteriskManager");
    if (strlen(cfgVal("AsteriskManagerEnabled"))) {
        if (atoi(cfgVal("AsteriskManagerEnabled"))) {
            asteriskManager->setChecked(true);
        } else {
            asteriskManager->setChecked(false);
        }
    }
    QToolTip::add(asteriskManager, "Enabling this will show the asterisk call manager at the bottom of the main window.");

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 1, 1);

    ml->addWidget(asteriskManager,0);
    ml->addStretch(1);
}

/*
 * ~GeneralSettings - Destructor.  Cleans up stuff.
 */
TelephonySettings::~TelephonySettings()
{
}

/*
 * validateSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int TelephonySettings::validateSettings()
{
    int retVal = 1;     // Assume success
    
    return retVal;
}

/*
 * saveSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int TelephonySettings::saveSettings()
{
    int retVal = 1;     // Assume success

    QString isChecked = "0";
    if (asteriskManager->isChecked()) isChecked = "1";
    updateCfgVal("AsteriskManagerEnabled", isChecked.ascii());
    
    return retVal;
}


