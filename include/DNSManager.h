/*
** $Id: DNSManager.h,v 1.2 2004/01/02 14:59:39 marc Exp $
**
***************************************************************************
**
** DNSManager - A tool that allows a TAA user to manage DNS entries for a
**              customer or a template.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2003, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DNSManager.h,v $
** Revision 1.2  2004/01/02 14:59:39  marc
** Added DNSUtils.h, removed some functions from DNSManager.h and put them
** into DNSUtils
**
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef DNSManager_included
#define DNSManager_included

#include <Qt3Support/q3listview.h>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QPushButton>

#include <TAATools.h>
#include <TAAWidget.h>
#include <ADB.h>
#include <DNSUtils.h>


// TAA DNS Manager class/widget
class DNSManager : public TAAWidget
{
    Q_OBJECT

public:
    DNSManager(QWidget *parent = NULL, const char *name = NULL);
    virtual ~DNSManager();

    int     setDomainName(const char *domain);
    
protected slots:
    int         addRR();
    int         delRR();
    int         delZone();

protected:
    void        updateSOASerial();

    int         currentZone;    // The Zone ID we are working on.
    QLabel      *domainName;
    QLabel      *domainType;
    QLabel      *lastModified;
    QLabel      *hostBoxLabel;
    QLineEdit   *hostBox;
    QLabel      *typeListLabel;
    QComboBox   *typeList;
    QLabel      *priSpinnerLabel;
    QSpinBox    *priSpinner;
    QLabel      *addressBoxLabel;
    QLineEdit   *addressBox;
    QPushButton *addButton;
    Q3ListView   *recordList;    // Host record list.
};

#endif // DNSManager_included

