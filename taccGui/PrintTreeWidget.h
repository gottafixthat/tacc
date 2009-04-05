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

#ifndef TAAPRINTTREEWIDGET_H
#define TAAPRINTTREEWIDGET_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QTreeWidget>

class PrintTreeWidget : public QObject
{
    Q_OBJECT

public:
    PrintTreeWidget(QTreeWidget *srcTree = NULL);
    virtual ~PrintTreeWidget();

    void        setTitle(const QString newTitle);
    void        setDateLine(const QString newDateLine);
    void        setPrintSelectedOnly(int);

    void        print();

protected:
    QString     myTitle;
    QString     myDateLine;
    int         myPrintSelectedOnly;
    QTreeWidget *myTree;
    int         showDateLine;

    int         prColWidths[20];
    int         prLeftMargin;
    int         prRightMargin;

    int         countChildren(QTreeWidgetItem *item);

    void printHeader(QPainter *p);
    void printFooter(QPainter *p, int PageNo, int totPages);
    void printRows(QPainter *p, int sItem, int eItem);

};

#endif

// vim: expandtab
