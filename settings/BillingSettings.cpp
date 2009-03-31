/*
** BillingSettings - Billing configuration options for TACC.
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
#include <q3filedialog.h>
#include <qfile.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3BoxLayout>
#include <TAATools.h>
#include "Cfg.h"
#include "BillingSettings.h"

/*
 * BillingSettings - Constructor.  Lays out the widget.
 */
BillingSettings::BillingSettings(QWidget *parent, const char *name) : TAAWidget(parent, name)
{
    setCaption("General Settings");

    builtInPrintedStatements = new QCheckBox(this, "builtInPrintedStatements");
    builtInPrintedStatements->setText("Use built in printed statement processing");
    QToolTip::add(builtInPrintedStatements, "If this option is checked, TACC will use its own\nbuilt in statement layout, otherwise TACC will\nuse LaTeX to typset and print statements using\nthe specified template.");

    QLabel *fromAddressLabel = new QLabel(this, "fromAddressLabel");
    fromAddressLabel->setText("From Address:");
    fromAddressLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    fromAddress = new QLineEdit(this, "latexFile");
    fromAddress->setText(cfgVal("StatementFromAddress"));

    if (!strlen(cfgVal("BuiltInPrintedStatements"))) builtInPrintedStatements->setChecked(true);
    else if (atoi(cfgVal("BuiltInPrintedStatements"))) builtInPrintedStatements->setChecked(true);
    else builtInPrintedStatements->setChecked(false);

    QLabel *latexFileLabel = new QLabel(this, "latexFileLabel");
    latexFileLabel->setText("LaTeX template:");
    latexFileLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    latexFile = new QLineEdit(this, "latexFile");
    latexFile->setText(cfgVal("StatementLatexFile"));
    QToolTip::add(latexFile, "If billing is set to use LaTeX instead of the\ninternally generated statements, this is the\nsource file that will be used.");

    chooseFileButton = new QPushButton(this, "chooseFileButton");
    chooseFileButton->setText("...");
    chooseFileButton->setFixedWidth(20);
    connect(chooseFileButton, SIGNAL(clicked()), this, SLOT(chooseLatexFile()));

    QLabel *emailBodyLabel = new QLabel(this, "latexBodyLabel");
    emailBodyLabel->setText("Email Body Template:");
    emailBodyLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    emailBodyFile = new QLineEdit(this, "emailBodyFile");
    emailBodyFile->setText(cfgVal("StatementLatexEmailBody"));
    QToolTip::add(emailBodyFile, "If billing is set to use LaTeX instead of the\ninternally generated statements, this is the\nsource file that will be used for the\nbody of the message.  A PDF file will be attached.");

    chooseBodyFileButton = new QPushButton(this, "chooseBodyFileButton");
    chooseBodyFileButton->setText("...");
    chooseBodyFileButton->setFixedWidth(20);
    connect(chooseBodyFileButton, SIGNAL(clicked()), this, SLOT(chooseEmailBodyFile()));

    QLabel *dateFormatLabel = new QLabel(this, "dateFormatLabel");
    dateFormatLabel->setText("Date Format:");
    dateFormatLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    dateFormat = new QLineEdit(this, "dateFormat");
    dateFormat->setText(cfgVal("LatexDateFormat"));
    QToolTip::add(dateFormat, "If billing is set to use LaTeX instead of the\ninternally generated statements, this how the dates are formatted.\nThis follows the Qt date format options.");

    qtyOneBlank = new QCheckBox(this, "qtyOneBlank");
    qtyOneBlank->setText("Use empty string for Quantity 1");
    QToolTip::add(qtyOneBlank, "When this option is enabled, if the quantity of a statement item\nis exactly 1, the quantity and price will be\nset to an empty string.");
    if (!strlen(cfgVal("StatementQtyOneBlank"))) qtyOneBlank->setChecked(false);
    else if (atoi(cfgVal("StatementQtyOneBlank"))) qtyOneBlank->setChecked(true);
    else qtyOneBlank->setChecked(false);

    latexDebug = new QCheckBox(this, "latexDebug");
    latexDebug->setText("Enable LaTeX test mode");
    QToolTip::add(latexDebug, "When this option is enabled, the LaTeX typesetter\nwill be run, but the statement will not be printed.\nThis will also leave the source file intact in /tmp.");
    if (!strlen(cfgVal("StatementLatexDebug"))) latexDebug->setChecked(false);
    else if (atoi(cfgVal("StatementLatexDebug"))) latexDebug->setChecked(true);
    else latexDebug->setChecked(false);

    doCCReceipts = new QCheckBox(this, "doCCReceipt");
    doCCReceipts->setText("Create credit card receipts/declined notices by default.");
    QString tmpReceipt = cfgVal("GenerateCCReceipts");
    if (tmpReceipt.toInt()) doCCReceipts->setChecked(true);
    else doCCReceipts->setChecked(false);

    Q3BoxLayout *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 1, 1);

    Q3GridLayout *gl = new Q3GridLayout(2,2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    int curRow = 0;

    gl->addWidget(fromAddressLabel,     curRow, 0);
    gl->addWidget(fromAddress,          curRow, 1);
    gl->setRowStretch(curRow, 0);

    curRow++;
    gl->addWidget(builtInPrintedStatements, curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    Q3BoxLayout *fbl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
    fbl->addWidget(latexFile, 1);
    fbl->addWidget(chooseFileButton, 0);

    gl->addWidget(latexFileLabel,       curRow, 0);
    gl->addLayout(fbl,                  curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    Q3BoxLayout *bbl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 1);
    bbl->addWidget(emailBodyFile, 1);
    bbl->addWidget(chooseBodyFileButton, 0);

    gl->addWidget(emailBodyLabel,       curRow, 0);
    gl->addLayout(bbl,                  curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;


    gl->addWidget(dateFormatLabel,      curRow, 0);
    gl->addWidget(dateFormat,           curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addWidget(qtyOneBlank,          curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addWidget(latexDebug,           curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addMultiCellWidget(new HorizLine(this, "hLine"),   curRow, curRow, 0, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addWidget(doCCReceipts, curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;


    ml->addLayout(gl, 1);
    ml->addStretch(1);  // This can go away if we add an item that gets stretch in the grid

    connect(builtInPrintedStatements, SIGNAL(toggled(bool)), this, SLOT(builtInPrintedStatementChanged(bool)));
    builtInPrintedStatementChanged(builtInPrintedStatements->isChecked());

}

/*
 * ~BillingSettings - Destructor.  Cleans up stuff.
 */
BillingSettings::~BillingSettings()
{
}

/*
 * validateSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int BillingSettings::validateSettings()
{
    int retVal = 1;     // Assume success

    if (!builtInPrintedStatements->isChecked()) {
        // Make sure that the latex file exists.
        QFile   file(latexFile->text());
        if (!file.exists()) {
            QMessageBox::critical(this, "Billing Settings", "The specified LaTeX template file could not be found.\nYou must specify an existing file name to use or use\nbuilt in statement processing.");
            retVal = 0;
        }
    }
    
    return retVal;
}


/*
 * saveSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int BillingSettings::saveSettings()
{
    int retVal = 1;     // Assume success

    updateCfgVal("StatementFromAddress", fromAddress->text().ascii());
    if (!builtInPrintedStatements->isChecked()) {
        // We've already validated the file.
        updateCfgVal("BuiltInPrintedStatements", "0");
        updateCfgVal("StatementLatexFile", (const char *)latexFile->text());
        updateCfgVal("StatementLatexEmailBody", (const char *)emailBodyFile->text());
        updateCfgVal("LatexDateFormat", (const char *)dateFormat->text());
        if (qtyOneBlank->isChecked()) {
            updateCfgVal("StatementQtyOneBlank", "1");
        } else {
            updateCfgVal("StatementQtyOneBlank", "0");
        }
        if (latexDebug->isChecked()) {
            updateCfgVal("StatementLatexDebug", "1");
        } else {
            updateCfgVal("StatementLatexDebug", "0");
        }
    } else {
        updateCfgVal("BuiltInPrintedStatements", "1");
    }
    // Credit card receipts
    if (doCCReceipts->isChecked()) {
        updateCfgVal("GenerateCCReceipts", "1");
    } else {
        updateCfgVal("GenerateCCReceipts", "0");
    }
    
    return retVal;
}

/*
 * builtInPrintedStatementChanged - Gets called when the value of
 * builtInPrintedStatements changes.  Enables or disables the LaTeX
 * file options.
 */
void BillingSettings::builtInPrintedStatementChanged(bool on)
{
    bool    tmpBool = true;
    if (on) tmpBool = false;

    latexFile->setEnabled(tmpBool);
    chooseFileButton->setEnabled(tmpBool);
    emailBodyFile->setEnabled(tmpBool);
    chooseBodyFileButton->setEnabled(tmpBool);
    dateFormat->setEnabled(tmpBool);
    qtyOneBlank->setEnabled(tmpBool);
    latexDebug->setEnabled(tmpBool);
}

/*
 * chooseLatexFile - Opens a file dialog to allow the user
 * to choose what file is going to be used.
 */
void BillingSettings::chooseLatexFile()
{
    QString     file;

    file = Q3FileDialog::getOpenFileName(latexFile->text(), "LaTeX files (*.tex);;All Files (*)", this, "open file dialog", "Choose a LaTeX file");

    if (file.length()) {
        latexFile->setText(file);
    }

}

/*
 * chooseEmailBodyFile - Opens a file dialog to allow the user
 * to choose what file is going to be used.
 */
void BillingSettings::chooseEmailBodyFile()
{
    QString     file;

    file = Q3FileDialog::getOpenFileName(emailBodyFile->text(), "Template files (*.tpl);;All Files (*)", this, "open file dialog", "Choose a template file");

    if (file.length()) {
        emailBodyFile->setText(file);
    }

}

