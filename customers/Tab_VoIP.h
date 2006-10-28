/*
***************************************************************************
**
** Tab_VoIP - Provides an interface for customer care personnel to add
** VoIP services for customers.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvanta Communications and R. Marc Lewis.
***************************************************************************
*/

#ifndef Tab_VoIP_included
#define Tab_VoIP_included

#include <qwidget.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qtextview.h>
#include <qstrlist.h>
#include <qpainter.h>
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
    QListView   *list;
    QPushButton *addButton;

public slots:
	void         refreshVoIPList(int);

protected slots:
    virtual void refreshCustomer(long);

private:
	long myCustID;
};

#endif // Tab_VoIP_included
