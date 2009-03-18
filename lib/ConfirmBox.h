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

#ifndef CONFIRMBOX_H
#define CONFIRMBOX_H

#include <qdialog.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qpushbutton.h>

class ConfirmBox : public QDialog
{
    Q_OBJECT
public:
    ConfirmBox(QWidget *parent = NULL, const char *name = NULL);
    virtual ~ConfirmBox();

    void    setTitle(const QString);
    void    setText(const QString);
    void    setConfirmText(const QString);
    void    setExtraConfirmation(bool);

protected:
    QLabel      *textArea;
    QCheckBox   *confirmation;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
    bool        doExtraConfirm;

protected slots:
    void    confirmChecked(bool);
};

#endif
