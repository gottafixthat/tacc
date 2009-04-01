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

#ifndef NoteEdit_included
#define NoteEdit_included

#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <Qt3Support/q3multilineedit.h>

#include <TAAWidget.h>

class NoteEdit : public TAAWidget
{
    Q_OBJECT

public:

    NoteEdit
    (
        QWidget* parent = NULL,
        const char* name = NULL,
        long CustID = 0,
        long InternalID = 0
    );

    virtual ~NoteEdit();

private slots:
    void    cancelNote();
    void    saveNote();

private:
	long	myCustID;
	long	myInternalID;
	
	char	myLoginID[64];
	char	myNoteType[64];
	char	myDate[64];
	char	myAddedBy[64];
	char	*myNotes;

protected:
    QComboBox       *noteType;
    QComboBox       *loginID;
    Q3MultiLineEdit  *noteText;
    QLabel          *dateText;
    QLabel          *addedByText;

};
#endif // NoteEdit_included

// vim: expandtab
