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

#ifndef CHECKPAYMENTSREPORT_H
#define CHECKPAYMENTSREPORT_H

#include <TACCReport.h>

class CheckPaymentsReport : public TACCReport
{
    Q_OBJECT

public:
    CheckPaymentsReport(QWidget* parent = NULL);
    virtual ~CheckPaymentsReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(QTreeWidgetItem *curItem, int col);
    
};
#endif

// vim: expandtab
