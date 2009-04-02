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

#ifndef RatePlans_included
#define RatePlans_included

#include <QtGui/QMenuBar>
#include <Qt3Support/q3listview.h>

#include <TAAWidget.h>

class RatePlans : public TAAWidget
{
    Q_OBJECT

public:

    RatePlans
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~RatePlans();

public slots:
    virtual void Hide();
    virtual void refreshList(int);

private slots:
    virtual void newRatePlan();
    virtual void editRatePlan();
    virtual void deleteRatePlan();
    void         editRatePlan(Q3ListViewItem *);
    
private:
    int		    internalIDColumn;

protected:
    QMenuBar    *menu;
    Q3ListView   *list;

};
#endif // RatePlans_included

// vim: expandtab
