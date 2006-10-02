/*
***************************************************************************
**
** DIDManager - An interface that allows an admin to manage DID's for
** our VoIP service.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvanta Communications and R. Marc Lewis.
***************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#include <ADB.h>

#include <qdatetm.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qapplication.h>

#include <TAA.h>
#include "DIDManager.h"
//#include "LogVoiceMail.h"

DIDManager::DIDManager
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "DID Manager" );

}

DIDManager::~DIDManager()
{
}


/**
  * DIDManagerAdd - Constructor for the DID Manager's Add DID class.
  */
DIDManagerAdd::DIDManagerAdd
(
	QWidget* parent,
	const char* name
) : TAAWidget( parent, name )
{
    setCaption( "DID Manager - Add DIDs" );

    QLabel *npaLabel = new QLabel(this, "npaLabel");
    npaLabel->setText("NPA/NXX:");
    npaLabel->setAlignment(AlignRight|AlignVCenter);

    npa = new QLineEdit(this, "npa");
    npa->setInputMask("999");
    npa->setMaxLength(3);
    npa->setMaximumWidth(npa->minimumSizeHint().width() * 2);

    QLabel *nxxLabel = new QLabel(this, "nxxLabel");
    nxxLabel->setText("/");
    nxxLabel->setAlignment(AlignRight|AlignVCenter);

    nxx = new QLineEdit(this, "nxx");
    nxx->setMaxLength(3);
    nxx->setInputMask("999");
    nxx->setMaximumWidth(nxx->minimumSizeHint().width() * 2);

    QLabel *startLabel = new QLabel(this, "startLabel");
    startLabel->setText("Starting Number:");

    numStart = new QLineEdit(this, "numStart");
    numStart->setInputMask("9999");
    numStart->setMaxLength(4);
    numStart->setMaximumWidth(numStart->minimumSizeHint().width() * 2);

    numStop = new QLineEdit(this, "numStop");
    numStop->setInputMask("9999");
    numStop->setMaxLength(4);
    numStop->setMaximumWidth(numStop->minimumSizeHint().width() * 2);

    QLabel *rateCenterLabel = new QLabel(this, "rateCenterLabel");
    rateCenterLabel->setText("Rate Center:");
    rateCenter = new QComboBox(true, this);

    messageArea = new QLabel(this, "messageArea");

    addButton = new QPushButton(this);
    addButton->setText("&Add");
    doneButton = new QPushButton(this);
    doneButton->setText("&Done");

    // Setup our layout now.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QBoxLayout  *didl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    didl->addWidget(npa, 0);
    didl->addWidget(nxxLabel, 0);
    didl->addWidget(nxx, 0);
    didl->addWidget(numStart, 0);
    didl->addWidget(numStop, 0);
    didl->addStretch(1);

    QGridLayout *gl = new QGridLayout(3, 5);
    gl->addWidget(npaLabel,             0, 0);
    gl->addLayout(didl,                 0, 1);
    gl->addWidget(startLabel,           1, 0);
    gl->addWidget(rateCenterLabel,      2, 0);
    gl->addWidget(rateCenter,           2, 1);

    ml->addLayout(gl, 1);

    // Add the button layout.
    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    bl->addWidget(messageArea, 0);
    bl->addStretch(1);
    bl->addWidget(addButton, 0);
    bl->addWidget(doneButton, 0);

    ml->addLayout(bl, 0);

}

/**
  * ~DIDManagerAdd - Destructor.
  */
DIDManagerAdd::~DIDManagerAdd()
{
}

/**
  * doneClicked - Gets called when the user hits the "Done" button.
  */
void DIDManagerAdd::doneClicked()
{
    return;
}

