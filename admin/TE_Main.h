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

// vim: expandtab


#ifndef TE_MAIN_H
#define TE_MAIN_H

#include <qwidgetstack.h>
#include <qtabbar.h>

#include <TAAWidget.h>

#include <TE_Message.h>
#include <TE_LoginTypes.h>
#include <TE_Cities.h>

/*!
 * \brief A widget to send a targeted email to customers.
 *
 * This widget gives a user the ability to send an email to all
 * or some customers.  It allows the user to filter on Login/Service
 * types or by city.
 */
class TE_Main : public TAAWidget
{
    Q_OBJECT

public:

    TE_Main(QWidget* parent = NULL, const char* name = NULL);
    virtual ~TE_Main();
    
    virtual void sendMessage();

protected slots:
    virtual void    cancelClicked()                 { delete this; }
    virtual void    closeEvent(QCloseEvent *)       { delete this; }
    virtual void    raiseTab(int);
    
protected:
    TE_Message      *tmessage;
    TE_LoginTypes   *tlogins;
    TE_Cities       *tcities;
    QTabBar         *theTabBar;
    QWidgetStack    *qws;

};

#endif
