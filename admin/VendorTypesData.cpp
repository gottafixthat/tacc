/**********************************************************************

	--- Dlgedit generated file ---

	File: VendorTypesData.cpp
	Last generated: Wed Sep 17 15:43:49 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by dlgedit.  All changes will be lost.

 *********************************************************************/

#include "VendorTypesData.h"

#define Inherited QWidget

#include <qpushbt.h>

VendorTypesData::VendorTypesData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	list = new QListBox( this, "list" );
	list->setGeometry( 10, 70, 450, 210 );
	connect( list, SIGNAL(selected(int)), SLOT(editVendorTypeL(int)) );
	list->setFrameStyle( 51 );
	list->setLineWidth( 2 );

	QPushButton* tmpQPushButton;
	tmpQPushButton = new QPushButton( this, "newBut" );
	tmpQPushButton->setGeometry( 0, 30, 60, 30 );
	{
        /*
		QColorGroup normal( QColor( QRgb(0) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(6316128) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
		QColorGroup disabled( QColor( QRgb(8421504) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(6316128) ), QColor( QRgb(10789024) ), QColor( QRgb(8421504) ), QColor( QRgb(12632256) ) );
		QColorGroup active( QColor( QRgb(0) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(6316128) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
		QPalette palette( normal, disabled, active );
		tmpQPushButton->setPalette( palette );
        */
	}
	connect( tmpQPushButton, SIGNAL(clicked()), SLOT(newVendorType()) );
	tmpQPushButton->setText( "New" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );

	tmpQPushButton = new QPushButton( this, "editBut" );
	tmpQPushButton->setGeometry( 60, 30, 60, 30 );
	connect( tmpQPushButton, SIGNAL(clicked()), SLOT(editVendorType()) );
	tmpQPushButton->setText( "Edit" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );

	tmpQPushButton = new QPushButton( this, "closeBut" );
	tmpQPushButton->setGeometry( 180, 30, 60, 30 );
	connect( tmpQPushButton, SIGNAL(clicked()), SLOT(Hide()) );
	tmpQPushButton->setText( "Close" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );

	tmpQPushButton = new QPushButton( this, "deleteBut" );
	tmpQPushButton->setGeometry( 120, 30, 60, 30 );
	connect( tmpQPushButton, SIGNAL(clicked()), SLOT(deleteVendorType()) );
	tmpQPushButton->setText( "Delete" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );

	menu = new QMenuBar( this, "menu" );
	menu->setGeometry( 0, 0, 470, 30 );
	menu->setFrameStyle( 34 );
	menu->setLineWidth( 2 );

	resize( 470, 290 );
}


VendorTypesData::~VendorTypesData()
{
}
void VendorTypesData::editVendorTypeL(int)
{
}
void VendorTypesData::newVendorType()
{
}
void VendorTypesData::editVendorType()
{
}
void VendorTypesData::Hide()
{
}
void VendorTypesData::deleteVendorType()
{
}
