/**
 * GeneralLedgerReport.h - Report class for check payments by
 * needed for Isomedia.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef GENERALLEDGERREPORT_H
#define GENERALLEDGERREPORT_H

#include "Report.h"

class GeneralLedgerReport : public Report
{
    Q_OBJECT

public:

    GeneralLedgerReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~GeneralLedgerReport();

    virtual void refreshReport();

public slots:
    virtual void    listItemSelected(QListViewItem *curItem);
    
};
#endif
