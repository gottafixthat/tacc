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
#include <qfiledialog.h>
#include <qfile.h>
#include <qmessagebox.h>
#include "Cfg.h"
#include "TAAConfig.h"
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

    if (!strlen(cfgVal("BuiltInPrintedStatements"))) builtInPrintedStatements->setChecked(true);
    else if (atoi(cfgVal("BuiltInPrintedStatements"))) builtInPrintedStatements->setChecked(true);
    else builtInPrintedStatements->setChecked(false);

    QLabel *latexFileLabel = new QLabel(this, "latexFileLabel");
    latexFileLabel->setText("LaTeX template:");
    latexFileLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    latexFile = new QLineEdit(this, "latexFile");
    latexFile->setText(cfgVal("StatementLatexFile"));
    QToolTip::add(latexFile, "If billing is set to use LaTeX instead of the\ninternally generated statements, this is the\nsource file that will be used.");

    QLabel *dateFormatLabel = new QLabel(this, "dateFormatLabel");
    dateFormatLabel->setText("Date Format:");
    dateFormatLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    dateFormat = new QLineEdit(this, "dateFormat");
    dateFormat->setText(cfgVal("LatexDateFormat"));
    QToolTip::add(dateFormat, "If billing is set to use LaTeX instead of the\ninternally generated statements, this how the dates are formatted.\nThis follows the Qt date format options.");

    chooseFileButton = new QPushButton(this, "chooseFileButton");
    chooseFileButton->setText("...");
    chooseFileButton->setFixedWidth(20);
    connect(chooseFileButton, SIGNAL(clicked()), this, SLOT(chooseLatexFile()));

    QBoxLayout *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 1, 1);

    QGridLayout *gl = new QGridLayout(2,2);
    gl->setColStretch(0, 0);
    gl->setColStretch(1, 1);
    int curRow = 0;

    gl->addWidget(builtInPrintedStatements, curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    QBoxLayout *fbl = new QBoxLayout(QBoxLayout::LeftToRight, 1);
    fbl->addWidget(latexFile, 1);
    fbl->addWidget(chooseFileButton, 0);

    gl->addWidget(latexFileLabel,       curRow, 0);
    gl->addLayout(fbl,                  curRow, 1);
    gl->setRowStretch(curRow, 0);
    curRow++;

    gl->addWidget(dateFormatLabel,      curRow, 0);
    gl->addWidget(dateFormat,           curRow, 1);
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

    if (!builtInPrintedStatements->isChecked()) {
        // We've already validated the file.
        updateCfgVal("BuiltInPrintedStatements", "0");
        updateCfgVal("StatementLatexFile", (const char *)latexFile->text());
        updateCfgVal("LatexDateFormat", (const char *)dateFormat->text());
    } else {
        updateCfgVal("BuiltInPrintedStatements", "1");
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
    dateFormat->setEnabled(tmpBool);
}

/*
 * chooseLatexFile - Opens a file dialog to allow the user
 * to choose what file is going to be used.
 */
void BillingSettings::chooseLatexFile()
{
    QString     file;

    file = QFileDialog::getOpenFileName(latexFile->text(), "LaTeX files (*.tex);;All Files (*)", this, "open file dialog", "Choose a LaTeX file");

    if (file.length()) {
        latexFile->setText(file);
    }

}

