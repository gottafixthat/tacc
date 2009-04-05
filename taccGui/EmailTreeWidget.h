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

#ifndef EMAILTREEWIDGET_H
#define EMAILTREEWIDGET_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeWidget>

#include <TAAWidget.h>
#include <EmailMessage.h>

class EmailTreeWidget : public TAAWidget
{
    Q_OBJECT

public:
    EmailTreeWidget(QTreeWidget *srcTree = NULL, QWidget *parent = NULL);
    virtual ~EmailTreeWidget();

    void        setTitle(const QString newTitle);
    void        setDateLine(const QString newDateLine);
    void        setEmailSelectedOnly(bool);

protected slots:
    void        emailReport();

protected:
    QString     myTitle;
    QString     myDateLine;
    bool        myEmailSelectedOnly;
    int         showDateLine;
    QTreeWidget *myTree;

    QLineEdit   *emailAddr;
    QCheckBox   *includePlainText;
    QCheckBox   *includeCSV;

    int         prColWidths[20];

};

#endif

// vim: expandtab
