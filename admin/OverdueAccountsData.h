/**********************************************************************

	--- Qt Architect generated file ---

	File: OverdueAccountsData.h
	Last generated: Mon Apr 24 16:26:20 2000

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef OverdueAccountsData_included
#define OverdueAccountsData_included

#include <TAAWidget.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qlistview.h>
#include <qprogressbar.h>

class OverdueAccountsData : public TAAWidget
{
    Q_OBJECT

public:

    OverdueAccountsData(QWidget *parent = NULL, const char *name = NULL);

    virtual ~OverdueAccountsData();

    protected slots:

    virtual void processSelections() =0;
    virtual void cancelOverdue() =0;
    virtual void selectNone() =0;
    virtual void selectAll() =0;

    protected:

    QRadioButton *noAction;
    QRadioButton *actionOne;
    QRadioButton *actionTwo;
    QLabel *accountsOverdue;
    QLabel *accountsSelected;
    QLabel *amountOverdue;
    QLabel *selectedOverdue;
    QProgressBar *progress;
    QListView *list;
    QLabel *statusBar;
};

#endif // OverdueAccountsData_included
