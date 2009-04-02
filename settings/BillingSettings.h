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

#ifndef BillingSettings_included
#define BillingSettings_included

#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>

#include <TAAWidget.h>

class BillingSettings : public TAAWidget
{
    Q_OBJECT

public:
    BillingSettings(QWidget *parent = NULL, const char *name = NULL);
    virtual ~BillingSettings();

    int             validateSettings();
    int             saveSettings();

protected:
    QCheckBox       *builtInPrintedStatements;
    QLineEdit       *fromAddress;
    QCheckBox       *latexDebug;
    QLineEdit       *latexFile;
    QLineEdit       *emailBodyFile;
    QLineEdit       *dateFormat;
    QCheckBox       *qtyOneBlank;
    QPushButton     *chooseFileButton;
    QPushButton     *chooseBodyFileButton;
    QCheckBox       *doCCReceipts;

protected slots:
    void            builtInPrintedStatementChanged(bool);
    void            chooseLatexFile();
    void            chooseEmailBodyFile();
};

#endif // GeneralSettings_included


// vim: expandtab
