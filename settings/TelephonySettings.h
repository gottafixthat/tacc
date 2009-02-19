/**
 * TelephonySettings.h - Configuration options for the Asterisk Manager
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef TelephonySettings_included
#define TelephonySettings_included

#include <qcheckbox.h>
#include "TAAWidget.h"

class TelephonySettings : public TAAWidget
{
    Q_OBJECT

public:
    TelephonySettings(QWidget *parent = NULL, const char *name = NULL);
    virtual ~TelephonySettings();

    int             validateSettings();
    int             saveSettings();

protected:
    QCheckBox       *asteriskManager;

};

#endif

