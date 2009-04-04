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
    DNSManager(QWidget *parent = NULL);
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

// vim: expandtab
