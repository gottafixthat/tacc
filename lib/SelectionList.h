/*
** $Id: SelectionList.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** SelectionList - A simple class that provides a selection list for the
**                 user to pick something.
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
** $Log: SelectionList.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/


#ifndef SelectionList_included
#define SelectionList_included

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <Qt3Support/q3listview.h>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>

class SelectionList : public QDialog
{
    Q_OBJECT

public:

    SelectionList
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~SelectionList();

    void setTitle(const char * newTitle);
    void setHeader(const char * newText);    
    
    void addColumn(const char *colName);
    
    Q3ListViewItem addItem(const char *,
      const char * = 0, const char * = 0, const char * = 0,
      const char * = 0, const char * = 0, const char * = 0,
      const char * = 0);
    Q3ListViewItem *getSelection();

protected slots:
    virtual void    itemSelected(Q3ListViewItem *);
    virtual void    acceptPressed();

protected:
    Q3ListView   *list;
    QLabel      *titleLabel;
    QPushButton *acceptButton;
    QPushButton *cancelButton;
    QLabel      *extraLabel;
    QComboBox   *extraList;
};

#endif // SelectionList_included
