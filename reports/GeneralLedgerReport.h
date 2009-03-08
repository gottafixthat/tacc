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
#include <TAAStructures.h>
#include <qstringlist.h>

/**
 * GeneralLedgerReport - Summary report for the GL.
 */
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

/**
 * GeneralLedgerDetailFilters - Filters widget to allow the user to
 * pick the account and columns that they want to show on the GL
 * detail report.
 */
class GeneralLedgerDetailFilters : public TAAWidget
{
    Q_OBJECT

public:
    GeneralLedgerDetailFilters(QWidget *parent = NULL, const char *name = NULL);
    ~GeneralLedgerDetailFilters();

    void                setAvailableColumns(const QStringList columnNames);
    void                setDisplayColumns(QStringList dispCols);
    QStringList         displayColumns();

signals:
    void                optionsUpdated();

protected slots:
    void                upClicked();
    void                downClicked();
    void                updateClicked();
    void                closeClicked();
    QStringMap          myColumnNames;

protected:
    QListBox            *columnList;
};

/**
 * GeneralLedgerDetailReport - GL Detail for a specific GL account.
 */
class GeneralLedgerDetailReport : public Report
{
    Q_OBJECT

public:
    GeneralLedgerDetailReport
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );
    virtual ~GeneralLedgerDetailReport();

    virtual void    refreshReport();
    void            setIntAccountNo(int intAccountNo);

public slots:
    virtual void    listItemSelected(QListViewItem *curItem);
    virtual void    editFilters();

protected:
    void            setReportColumns();
    int             myIntAccountNo;
    int             myCustomerIDCol;
    QStringMap      availableColumns;
    QStringList     columnNames;
    GeneralLedgerDetailFilters  *filters;

};

#endif
