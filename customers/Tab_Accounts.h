/*
** $Id: Tab_Accounts.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** Tab_Accounts.h - A concept that never got finished...
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: Tab_Accounts.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef Tab_Accounts_included
#define Tab_Accounts_included

#include "Tab_AccountsData.h"



class Tab_Accounts : public Tab_AccountsData
{
    Q_OBJECT

public:

    Tab_Accounts
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long  CustID = 0
    );

    virtual ~Tab_Accounts();

    void    refreshAccountsList(int);
    
private:
    void    getSubscriptions(const char *LoginID, QListViewItem *parent);

    long    myCustID;

};
#endif // Tab_Accounts_included
