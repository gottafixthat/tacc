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

#ifndef DOMAINCHECKLIST_H
#define DOMAINCHECKLIST_H

#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlineedit.h>

#include <TAAWidget.h>

class DomainChecklist : public TAAWidget
{
    Q_OBJECT

public:

    DomainChecklist(QWidget* parent = NULL, const char* name = NULL, long DomainID = 0);
    virtual ~DomainChecklist();

    void refreshCheckList();
    
    void saveChecklist();
    
protected slots:
    virtual void cancelClicked();
    virtual void saveClicked();
    virtual void updateClicked();

protected:
    long        myDomainID;
    long        myCustID;
    QLabel      *customerID;
    QLabel      *loginID;
    QLabel      *customerName;
    QLabel      *domainName;
    QCheckBox   *hostmasterSubmit;
    QLabel      *hostmastDate;
    QCheckBox   *internicSubmit;
    QLabel      *nicReqDate;
    QCheckBox   *dnsDone;
    QLabel      *dnsDate;
    QCheckBox   *mailDone;
    QLabel      *mailDate;
    QCheckBox   *vserverDone;
    QLabel      *vserverDate;
    QComboBox   *serverList;
    QComboBox   *processList;
    QLineEdit   *ipAddress;
    QCheckBox   *internicDone;
    QLabel      *nicDoneDate;
    QLineEdit   *nicAdminID;
    QLineEdit   *nicBillID;
    QCheckBox   *domainDone;
    QLabel      *doneDate;

};
#endif // DomainChecklist_included

// vim: expandtab

