/**********************************************************************

	--- Qt Architect generated file ---

	File: AddBaseTypeLinkData.cpp
	Last generated: Sat Dec 13 05:27:12 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include "AddBaseTypeLinkData.h"

#define Inherited QDialog

#include <qlabel.h>

AddBaseTypeLinkData::AddBaseTypeLinkData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, TRUE, 0 )
{
	baseItemList = new QListBox( this, "ListBox_1" );
	baseItemList->setGeometry( 20, 60, 310, 180 );
	baseItemList->setMinimumSize( 10, 10 );
	baseItemList->setMaximumSize( 32767, 32767 );
	connect( baseItemList, SIGNAL(selected(int)), SLOT(itemSelected(int)) );
	baseItemList->setFrameStyle( 51 );
	baseItemList->setLineWidth( 2 );
	baseItemList->setMultiSelection( FALSE );

	QLabel* dlgedit_Label_1;
	dlgedit_Label_1 = new QLabel( this, "Label_1" );
	dlgedit_Label_1->setGeometry( 20, 10, 310, 40 );
	dlgedit_Label_1->setMinimumSize( 10, 10 );
	dlgedit_Label_1->setMaximumSize( 32767, 32767 );
	dlgedit_Label_1->setText( "Select the Base Type to link to this Billable Item." );
	dlgedit_Label_1->setAlignment( 289 );
	dlgedit_Label_1->setMargin( -1 );

	acceptButton = new QPushButton( this, "PushButton_1" );
	acceptButton->setGeometry( 70, 250, 100, 30 );
	acceptButton->setMinimumSize( 10, 10 );
	acceptButton->setMaximumSize( 32767, 32767 );
	acceptButton->setText( "&Accept" );
	acceptButton->setAutoRepeat( FALSE );
	acceptButton->setAutoResize( FALSE );

	cancelButton = new QPushButton( this, "PushButton_2" );
	cancelButton->setGeometry( 190, 250, 100, 30 );
	cancelButton->setMinimumSize( 10, 10 );
	cancelButton->setMaximumSize( 32767, 32767 );
	cancelButton->setText( "&Cancel" );
	cancelButton->setAutoRepeat( FALSE );
	cancelButton->setAutoResize( FALSE );

	resize( 350,300 );
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );
}


AddBaseTypeLinkData::~AddBaseTypeLinkData()
{
}
void AddBaseTypeLinkData::itemSelected(int)
{
}
