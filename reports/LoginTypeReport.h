/*
** $Id: LoginTypeReport.h,v 1.1 2003/12/07 01:47:05 marc Exp $
**
***************************************************************************
**
** LoginTypeReport.h - A "drill down" report that shows a list of a
**                     particular login type.
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
** $Log: LoginTypeReport.h,v $
** Revision 1.1  2003/12/07 01:47:05  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef LoingTypeReport_H
#define LoginTypeReport_H

#include "Report.h"

class LoginTypeReport : public Report
{
    Q_OBJECT

public:
    LoginTypeReport(QWidget * parent = NULL, const char *name = NULL);
    virtual ~LoginTypeReport();

    virtual void refreshReport();

    void    setLoginType(int newLoginType);

protected:
    int     loginType;
    int     custIDColumn;
    bool    showActive;
    bool    showInactive;
    bool    showWiped;

protected slots:
    virtual void listItemSelected(Q3ListViewItem *curItem);
};


#endif  // LoginTypeReport_H

