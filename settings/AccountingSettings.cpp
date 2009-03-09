/**
 * AccountingSettings.cpp - Configuration options for certain GL options.
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

#include <ADB.h>
#include <TAAConfig.h>
#include <Cfg.h>
#include <AccountingSettings.h>

/**
 * AccountingSettings()
 *
 * Constructor.  Lays out the widget.
 */
AccountingSettings::AccountingSettings(QWidget *parent, const char *name) : TAAWidget(parent, name)
{
    setCaption("Accounting Settings");

    // All of the combo boxes have the same contents so we need only one
    // index for all of them
    acctNoIDX = NULL;

    // Create the widgets first.
    acctsRecvAccount = new QComboBox(false, this, "acctsRecvAccount");
    undepositedFundsAccount = new QComboBox(false, this, "undepositedFundsAccount");
    undepositedCCAccount = new QComboBox(false, this, "undepositedCCAccount");
    collectionsAccount = new QComboBox(false, this, "collectionsAccount");
    
    QString tmpStr;
    ADB     DB;
    int     collAcct = 0;
    int     arAcct = 0;
    int     ufAcct = 0;
    int     ufccAcct = 0;

    if (strlen(cfgVal("UndepositedFundsAcct"))) ufAcct = atoi(cfgVal("UndepositedFundsAcct"));
    if (strlen(cfgVal("UndepositedCCAcct"))) ufccAcct = atoi(cfgVal("UndepositedCCAcct"));
    if (strlen(cfgVal("CollectionsAccount"))) collAcct = atoi(cfgVal("CollectionsAccount"));
    if (strlen(cfgVal("AcctsRecvAccount"))) arAcct = atoi(cfgVal("AcctsRecvAccount"));

    DB.query("select IntAccountNo, AccountNo, AcctName from Accounts order by AccountNo, AcctName");
    acctNoIDX = new int[DB.rowCount+2];
    int curIDX = 0;
    int intAcctNo;
    while(DB.getrow()) {
        intAcctNo = atoi(DB.curRow["IntAccountNo"]);
        acctNoIDX[curIDX] = intAcctNo;
        tmpStr = DB.curRow["AccountNo"];
        tmpStr += " ";
        tmpStr += DB.curRow["AcctName"];
        acctsRecvAccount->insertItem(tmpStr);
        undepositedFundsAccount->insertItem(tmpStr);
        undepositedCCAccount->insertItem(tmpStr);
        collectionsAccount->insertItem(tmpStr);
        
        if (arAcct == intAcctNo) acctsRecvAccount->setCurrentItem(curIDX);
        if (ufAcct == intAcctNo) undepositedFundsAccount->setCurrentItem(curIDX);
        if (ufccAcct == intAcctNo) undepositedCCAccount->setCurrentItem(curIDX);
        if (collAcct == intAcctNo) collectionsAccount->setCurrentItem(curIDX);

        curIDX++;
    }
    
    QLabel  *acctsRecvAccountLabel = new QLabel(this, "acctsRecvAccountLabel");
    acctsRecvAccountLabel->setText("Accounts Receivable Account:");
    acctsRecvAccountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    QLabel  *undepositedFundsAccountLabel = new QLabel(this, "undepositedFundsAccountLabel");
    undepositedFundsAccountLabel->setText("Undeposited Funds Account:");
    undepositedFundsAccountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    QLabel  *undepositedCCAccountLabel = new QLabel(this, "undepositedCCAccountLabel");
    undepositedCCAccountLabel->setText("Credit Card Payment Account:");
    undepositedCCAccountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    QLabel  *collectionsAccountLabel = new QLabel(this, "collectionsAccountLabel");
    collectionsAccountLabel->setText("Collections Account:");
    collectionsAccountLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    // Our main layout is a box.
    QBoxLayout  *ml = new QBoxLayout(this, QBoxLayout::TopToBottom, 3);

    // Now create a grid layout to hold the input boxes
    QGridLayout *gl = new QGridLayout(3, 2);
    int curRow = 0;
    gl->addWidget(acctsRecvAccountLabel,        curRow, 0);
    gl->addWidget(acctsRecvAccount,             curRow, 1);
    curRow++;
    gl->addWidget(undepositedFundsAccountLabel, curRow, 0);
    gl->addWidget(undepositedFundsAccount,      curRow, 1);
    curRow++;
    gl->addWidget(undepositedCCAccountLabel,    curRow, 0);
    gl->addWidget(undepositedCCAccount,         curRow, 1);
    curRow++;
    gl->addWidget(collectionsAccountLabel,      curRow, 0);
    gl->addWidget(collectionsAccount,           curRow, 1);
    curRow++;
    gl->setColStretch(0,0);
    gl->setColStretch(1,1);

    ml->addLayout(gl, 0);
    ml->addStretch(1);

}

/*
 * ~GeneralSettings - Destructor.  Cleans up stuff.
 */
AccountingSettings::~AccountingSettings()
{
}

/*
 * validateSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int AccountingSettings::validateSettings()
{
    int retVal = 1;     // Assume success
    
    return retVal;
}

/*
 * saveSettings - Slot that gets called when the user clicks
 * the Save button.
 */
int AccountingSettings::saveSettings()
{
    int retVal = 1;     // Assume success
    QString tmpStr;


    tmpStr.setNum(acctNoIDX[undepositedFundsAccount->currentItem()]);
    updateCfgVal("UndepositedFundsAcct", tmpStr.ascii());

    tmpStr.setNum(acctNoIDX[undepositedCCAccount->currentItem()]);
    updateCfgVal("UndepositedCCAcct", tmpStr.ascii());

    tmpStr.setNum(acctNoIDX[collectionsAccount->currentItem()]);
    updateCfgVal("CollectionsAccount", tmpStr.ascii());

    tmpStr.setNum(acctNoIDX[acctsRecvAccount->currentItem()]);
    updateCfgVal("AcctsRecvAccount", tmpStr.ascii());

    return retVal;
}


