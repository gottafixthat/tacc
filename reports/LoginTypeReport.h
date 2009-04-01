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


// vim: expandtab
