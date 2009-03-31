/*
** $Id: CustomerSearch.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** CustomerSearch - A standalone widget that allows the user to search
**                  through the customer database and pick a customer.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2001, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: CustomerSearch.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef CUSTOMERSEARCH_H
#define CUSTOMERSEARCH_H

#include <TAAWidget.h>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <Qt3Support/q3listview.h>
#include <QtCore/QMap>

typedef QMap<long, long> loadedCustMap;


class CustomerSearch : public TAAWidget
{
    Q_OBJECT

public:
    CustomerSearch(QWidget *parent = NULL, const char *name = NULL, Qt::WFlags f = 0);
    virtual ~CustomerSearch();

    long            currentCustomer();
    void            takeFocus();

signals:
    void            customerHighlighted(long);
    void            customerSelected(long);

protected:
    QLineEdit       *searchText;
    QPushButton     *searchButton;
    Q3ListView       *custList;

protected slots:
    void            startSearch();
    void            clearSearch();
    void            itemHighlighted(Q3ListViewItem *);
    void            itemSelected(Q3ListViewItem *);

private:
    loadedCustMap   loadedCusts;
};




#endif // CUSTOMERSEARCH_H
