/*
** $Id: DNSTemplatePicker.h,v 1.3 2004/01/02 23:56:14 marc Exp $
**
***************************************************************************
**
** DNSTemplatePicker - Allows the user to pick a DNS Template.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2004, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: DNSTemplatePicker.h,v $
** Revision 1.3  2004/01/02 23:56:14  marc
** Domain Template Editor and SQL based DNS is (for the most part) fully functional and ready to use.
**
** Revision 1.2  2004/01/02 14:59:01  marc
** DNSTemplatePicker complete, added DNSUtils
**
** Revision 1.1  2004/01/01 01:57:54  marc
** *** empty log message ***
**
**
*/

#ifndef DNSTemplatePicker_Included
#define DNSTemplatePicker_Included

#include <qwidget.h>
#include <qdialog.h>
#include <qlistview.h>
#include <qpushbutton.h>

class DNSTemplatePicker : public QDialog
{
    Q_OBJECT

public:
    DNSTemplatePicker(QWidget *parent = NULL, const char *name = NULL);
    virtual ~DNSTemplatePicker();

    long        getTemplateID();

protected:
    QListView   *templateList;
    QPushButton *selectButton;
    QPushButton *cancelButton;

protected slots:
    void        refreshList();
    void        itemSelected(QListViewItem *);
    void        itemDoubleClicked(QListViewItem *);

};

#endif
