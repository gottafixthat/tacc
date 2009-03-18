/**
 * ConfirmBox.h - A modal dialog that makes the user take one extra
 * step to confirm they want to perform an action by optionally clicking
 * an extra checkbox.
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
#include <qstyle.h>

#include <TAAWidget.h>
#include <ConfirmBox.h>

/**
 * ConfirmBox()
 *
 * Constructor.
 */
ConfirmBox::ConfirmBox(QWidget *parent, const char *name) :
    QDialog(parent, name, true)
{
    setCaption("Confirmation");
    doExtraConfirm = true;

    QLabel  *iconLabel = new QLabel(this, "iconLabel");
    iconLabel->setPixmap(QApplication::style().stylePixmap(QStyle::SP_MessageBoxQuestion));
    iconLabel->setAlignment(AlignTop|AlignLeft);

    textArea = new QLabel(this, "textArea");
    textArea->setAlignment(AlignLeft|AlignTop);

    confirmation = new QCheckBox(this, "confirmation");
    confirmation->setText("Check this box to confirm this action before proceeding");
    confirmation->setChecked(false);
    connect(confirmation, SIGNAL(toggled(bool)), this, SLOT(confirmChecked(bool)));

    confirmButton = new QPushButton(this, "confirmButton");
    confirmButton->setText("C&onfirm");
    confirmButton->setEnabled(false);
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(accept()));
    
    cancelButton = new QPushButton(this, "cancelButton");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    cancelButton->setFocus();
    //cancelButton->setDefault();

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3, 3);

    QBoxLayout *ltrl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    ltrl->addWidget(iconLabel, 0);

    QBoxLayout *ttbl = new QBoxLayout(QBoxLayout::TopToBottom, 3);
    ttbl->addWidget(textArea, 1);
    ttbl->addSpacing(10);
    ttbl->addWidget(confirmation, 0);
    ltrl->addLayout(ttbl, 1);

    ml->addLayout(ltrl, 1);
    ml->addWidget(new HorizLine(this, "hLine"), 0);

    QBoxLayout *bl = new QBoxLayout(QBoxLayout::LeftToRight, 3);
    bl->addStretch(1);
    bl->addWidget(confirmButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);
}

/**
 * ~ConfirmBox()
 *
 * Destructor.
 */
ConfirmBox::~ConfirmBox()
{
}

/**
 * setTitle()
 *
 * Sets the caption for the widget.
 */
void ConfirmBox::setTitle(const QString newTitle)
{
    setCaption(newTitle);
}

/**
 * setText()
 *
 * Sets the text in the text area.
 */
void ConfirmBox::setText(const QString newText)
{
    textArea->setText(newText);
}

/**
 * setConfirmText()
 *
 * Sets the text in the text area.
 */
void ConfirmBox::setConfirmText(const QString newText)
{
    confirmation->setText(newText);
}

/**
 * extraConfirmation()
 *
 * Sets whether or not we need extra confirmation.
 */
void ConfirmBox::setExtraConfirmation(bool extra)
{
    doExtraConfirm = extra;
    if (extra) {
        confirmation->setChecked(false);
    } else {
        confirmation->setChecked(true);
    }
}

/**
 * confirmChecked()
 *
 * Slot that gets called when the user clicks the confirm
 * checkbox.
 */
void ConfirmBox::confirmChecked(bool confirm)
{
    confirmButton->setEnabled(confirm);
}

