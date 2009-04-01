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


#ifndef ELT_Flags_included
#define ELT_Flags_included

#include <QtCore/QString>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class ELT_Flags : public TAAWidget
{
    Q_OBJECT

public:

    ELT_Flags
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ELT_Flags();

    void    setLoginTypeID(long newLoginTypeID);
    
protected:
    Q3ListView       *flagList;
    QLineEdit       *flagValue;
    QPushButton     *setValueButton;
    QPushButton     *addButton;
    QPushButton     *removeButton;

protected slots:
    virtual void removeFlag();
    virtual void addFlag();
    virtual void itemSelected(Q3ListViewItem *curItem);
    virtual void flagValueChanged(const QString &);
    virtual void setValue();
    
private:
    long    myLoginTypeID;

};
#endif // ELT_Flags_included

// vim: expandtab
