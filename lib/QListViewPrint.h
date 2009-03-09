/**
 * QListViewPrint.h - A utility class that will "pretty print" any QListView
 * that is passed to it.
 *
 * Known Limitations - It won't indent children at this point.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef QLISTVIEWPRINT_H
#define QLISTVIEWPRINT_H

#include <qobject.h>
#include <qstring.h>
#include <qlistview.h>

class QListViewPrint : public QObject
{
    Q_OBJECT

public:
    QListViewPrint(QListView *srcList = NULL);
    virtual ~QListViewPrint();

    void        setTitle(const QString newTitle);
    void        setDateLine(const QString newDateLine);
    void        setPrintSelectedOnly(int);

    void        print();

protected:
    QString     myTitle;
    QString     myDateLine;
    int         myPrintSelectedOnly;
    QListView   *myList;
    int         showDateLine;

    int         prColWidths[20];
    int         prLeftMargin;
    int         prRightMargin;

    void printHeader(QPainter *p);
    void printFooter(QPainter *p, int PageNo, int totPages);
    QListViewItem * printRows(QPainter *p, QListViewItem *startItem);

};

#endif
