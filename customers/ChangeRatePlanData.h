/**********************************************************************

	--- Qt Architect generated file ---

	File: ChangeRatePlanData.h
	Last generated: Thu Jul 23 19:19:55 1998

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef ChangeRatePlanData_included
#define ChangeRatePlanData_included

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbt.h>
#include <qlined.h>
#include <qcombo.h>

class ChangeRatePlanData : public QDialog
{
    Q_OBJECT

public:

    ChangeRatePlanData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ChangeRatePlanData();

public slots:


protected slots:

    virtual void saveRatePlanChange();
    virtual void cancelRatePlanChange();

protected:
    QComboBox* ratePlanList;
    QLineEdit* effectiveDate;
    QPushButton* saveButton;
    QPushButton* cancelButton;
    QLabel* customerLabel;

};

#endif // ChangeRatePlanData_included
