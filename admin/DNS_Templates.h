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

#ifndef DNS_Templates_Included
#define DNS_Templates_Included

#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class DNS_Templates : public TAAWidget
{
    Q_OBJECT
public:
    DNS_Templates(QWidget *parent = NULL, const char *name = NULL);
    virtual ~DNS_Templates();

protected:
    Q3ListView       *templateList;
    // SOA Edit Fields
    QLineEdit       *templateName;
    QSpinBox        *refreshSpin;
    QSpinBox        *retrySpin;
    QSpinBox        *expireSpin;
    QSpinBox        *minimumSpin;
    QSpinBox        *ttlSpin;
    QCheckBox       *isActive;
    QCheckBox       *propogate;
    QCheckBox       *allowCustEdit;
    QPushButton     *saveSOAButton;
    // RR Edit Fields
    Q3ListView       *rrList;
    QLineEdit       *hostName;
    QComboBox       *typeList;
    QSpinBox        *priSpin;
    QLineEdit       *ipAddress;
    QSpinBox        *rrTTLSpin;
    QPushButton     *addRRButton;
    QPushButton     *delRRButton;

    
    QPushButton     *newButton;
    QPushButton     *deleteButton;
    QPushButton     *closeButton;

    void            disableEditElements();

protected slots:
    void            refreshList();
    void            templateSelected(Q3ListViewItem *);
    void            newTemplate();
    void            delTemplate();
    void            saveSOA();
    void            addRR();
    void            delRR();

private:
    long            curTemplateID;

};

#endif


// vim: expandtab
