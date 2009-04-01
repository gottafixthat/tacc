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

#ifndef DomainAliases_included
#define DomainAliases_included

#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class DomainAliases : public TAAWidget
{
    Q_OBJECT

public:
    DomainAliases (QWidget *parent = NULL, const char *name = NULL);
    ~DomainAliases();

    void setCustomerID(long);

protected:
    Q3ListView   *aliasList;
    QLineEdit   *address;
    QComboBox   *domainList;
    QComboBox   *loginList;
    QPushButton *addButton;
    QPushButton *refreshButton;
    QPushButton *autoButton;
    QPushButton *delButton;
    QPushButton *closeButton;
    long        myCustID;
    int         virtIDCol;

protected slots:
    void        refreshAlias();
    void        addAlias();
    void        delAlias();
    void        autoAlias();
};


#endif // DomainAliases_included


// vim: expandtab
