/**********************************************************************

	--- Qt Architect generated file ---

	File: TE_MessageData.h
	Last generated: Tue Jul 27 12:05:27 1999

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef TE_MessageData_included
#define TE_MessageData_included

#include <qwidget.h>
#include <qlined.h>
#include <qmultilineedit.h>

class TE_MessageData : public QWidget
{
    Q_OBJECT

public:

    TE_MessageData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~TE_MessageData();

public slots:


protected slots:

    virtual void msgTextChanged();
    virtual void subjectChanged(const char*);
    virtual void fromChanged(const char*);

protected:
    QLineEdit* fromLine;
    QLineEdit* msgSubject;
    QMultiLineEdit* msgText;

};

#endif // TE_MessageData_included
