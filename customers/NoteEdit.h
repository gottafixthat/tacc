/*
** $Id: NoteEdit.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** NoteEdit - Gives the user the ability to add/edit customer notes.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2002, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: NoteEdit.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef NoteEdit_included
#define NoteEdit_included

#include <TAAWidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qmultilineedit.h>

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
    QMultiLineEdit  *noteText;
    QLabel          *dateText;
    QLabel          *addedByText;

};
#endif // NoteEdit_included
