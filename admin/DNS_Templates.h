/*
** $Id: DNS_Templates.h,v 1.1 2003/12/31 21:35:26 marc Exp $
**
***************************************************************************
**
** DNS_Templates - A DNS template editor for SQL based DNS entries.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DNS_Templates.h,v $
** Revision 1.1  2003/12/31 21:35:26  marc
** Fully functional.  Could use a bit more polish, but then what software couldn't.
**
**
*/

#ifndef DNS_Templates_Included
#define DNS_Templates_Included

#include <TAAWidget.h>
#include <qwidget.h>
#include <q3listview.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>

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

