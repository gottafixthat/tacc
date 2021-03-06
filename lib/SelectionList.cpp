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

#include <QtGui/QLayout>
#include <Qt3Support/Q3BoxLayout>
#include <QtGui/QLabel>
#include "SelectionList.h"

SelectionList::SelectionList
(
	QWidget* parent,
	const char* name
) : QDialog ( parent, name )
{
	setCaption( "Select an Item" );

    titleLabel = new QLabel(this, "Title Label");
    titleLabel->setText("Please select an option.");
    titleLabel->setAlignment(Qt::AlignTop|Qt::AlignHCenter);

    list = new Q3ListView(this, "Selection List");
    list->setAllColumnsShowFocus(true);

    extraLabel = new QLabel(this, "Extra Label");
    extraLabel->setText("");
    extraLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    extraList = new QComboBox(false, this, "Extra List");

    acceptButton = new QPushButton(this, "Accept Button");
    acceptButton->setText("&Accept");
    connect(acceptButton, SIGNAL(clicked()), SLOT(accept()));
    
    cancelButton = new QPushButton(this, "Cancel Button");
    cancelButton->setText("&Cancel");
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));


    // Create the layout for this box.  _Very_ simple.  Top down box
    // with a button box at the bottom.
    
    Q3BoxLayout  *ml = new Q3BoxLayout(this, Q3BoxLayout::TopToBottom, 3, 3);
    ml->addWidget(titleLabel, 0);
    ml->addWidget(list, 1);
    
    Q3BoxLayout *bl = new Q3BoxLayout(Q3BoxLayout::LeftToRight, 3);
    bl->addWidget(extraLabel, 0);
    bl->addWidget(extraList, 1);
    bl->addSpacing(10);
    bl->addWidget(acceptButton, 0);
    bl->addWidget(cancelButton, 0);

    ml->addLayout(bl, 0);

    extraLabel->hide();
    extraList->hide();
	
}


SelectionList::~SelectionList()
{
}

/*
** setTitle - Sets the title on the window.
*/

void SelectionList::setTitle(const char * newTitle)
{
    setCaption(newTitle);
}


/*
** setHeader - Sets the header text on the window.
*/

void SelectionList::setHeader(const char * newText)
{
    titleLabel->setText(newText);
}

void SelectionList::addColumn(const char *colName)
{
    list->addColumn(colName);
}



/*
** addItem - Adds an item to the list.
*/

Q3ListViewItem SelectionList::addItem(const char * i1, const char *i2, const char *i3, const char *i4, const char *i5, const char *i6, const char *i7, const char *i8)
{
    return new Q3ListViewItem(list, i1, i2, i3, i4, i5, i6, i7, i8);
}

/*
** getSelection - Shows the list and gets the selection from the user.
*/

Q3ListViewItem *SelectionList::getSelection()
{
    exec();
    if (result() == Accepted) {
        return(list->currentItem());
    }
    return(NULL);
/*
    list->setCurrentItem(0);
    show();
    if (result() == Accepted) {
        return(list->currentItem());
    }
    return(-1);
*/
}


void SelectionList::itemSelected(Q3ListViewItem*)
{
}
void SelectionList::acceptPressed()
{
}

// vim: expandtab

