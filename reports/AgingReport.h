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

#ifndef AGINGREPORT_H
#define AGINGREPORT_H

#include "Report.h"

class AgingReport : public Report
{
    Q_OBJECT

public:

    AgingReport (QWidget* parent = NULL, const char* name = NULL);
    virtual ~AgingReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(Q3ListViewItem *curItem);

protected:
    int     custIDCol;
    
};
#endif

// vim: expandtab
