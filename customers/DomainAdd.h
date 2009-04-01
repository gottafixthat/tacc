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

#ifndef DomainAdd_included
#define DomainAdd_included

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>

#include <TAAWidget.h>

class DomainAdd : public TAAWidget
{
    Q_OBJECT

public:

    DomainAdd
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustomerID = 0
    );

    virtual ~DomainAdd();
    
public slots:
    virtual void saveDomain();
    virtual void cancelDomain();
    
protected slots:
    void        typeChanged(int);

protected:
    QLabel      *customerID;
    QLabel      *customerName;
    QComboBox   *domainType;
    QComboBox   *loginID;
    QLineEdit   *domainName;
    QCheckBox   *autoDNS;
    QCheckBox   *autoAliases;
    QCheckBox   *autoHosting;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    void        setMostPopular();

private:
    long        myCustID;
    int         *domainTypeIDX;
    
signals:
    void        domainSaved(int);

};

#endif // DomainAdd_included

// vim: expandtab
