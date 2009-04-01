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

#ifndef Tab_VoIP_included
#define Tab_VoIP_included

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QPainter>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3strlist.h>

#include <BlargDB.h>
#include <TAAWidget.h>

class Tab_VoIP : public TAAWidget
{
    Q_OBJECT

public:

    Tab_VoIP
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0
    );

    virtual ~Tab_VoIP();

protected:
    Q3ListView   *list;
    QPushButton *addButton;

public slots:
	void         refreshVoIPList(int);

protected slots:
    virtual void refreshCustomer(long);
    virtual void    addVoIPService();

private:
	long myCustID;
};

#endif // Tab_VoIP_included

// vim: expandtab
