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

#ifndef TE_CITIES_H
#define TE_CITIES_H

#include <qcheckbox.h>
#include <qlistview.h>

#include <TAAWidget.h>

/*!
 * \brief This allows the targeted email widget to narrow down
 * the message it sends to specific cities.
 */
class TE_Cities : public TAAWidget
{
    Q_OBJECT

public:

    TE_Cities(QWidget* parent = NULL, const char* name = NULL);

    virtual ~TE_Cities();
    
    void    loadCities();
    int     allCities();
    int     isIncluded(const char *City);
    
protected slots:
    virtual void    allCitiesClicked();
    virtual void    closeEvent(QCloseEvent *)       { delete this; }

protected:
    QCheckBox   *allCitiesButton;
    QListView   *cityList;

};
#endif
// vim: expandtab
