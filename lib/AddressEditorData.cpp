/**********************************************************************

	--- Qt Architect generated file ---

	File: AddressEditorData.cpp
	Last generated: Tue Feb 22 13:17:44 2000

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "AddressEditorData.h"

#include <qlabel.h>
#include <qvbuttongroup.h>

AddressEditorData::AddressEditorData(QWidget *parent, const char *name)
                  : QDialog(parent, name, FALSE, 0)
{
    typeLabel = new QLabel(this, "TypeLabel");
    typeLabel->setGeometry(5, 5, 54, 13);
    typeLabel->setMinimumSize(0, 0);
    typeLabel->setMaximumSize(32767, 32767);
    typeLabel->setFocusPolicy(QWidget::NoFocus);
    typeLabel->setBackgroundMode(QWidget::PaletteBackground);
    typeLabel->setFrameStyle( 0 );
    typeLabel->setLineWidth( 1 );
    typeLabel->setMidLineWidth( 0 );
    typeLabel->QFrame::setMargin( -1 );
    typeLabel->setText( "Customer:" );
    typeLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    typeLabel->setMargin( -1 );

    QLabel *qtarch_TagLabel = new QLabel(this, "TagLabel");
    qtarch_TagLabel->setGeometry(5, 33, 23, 59);
    qtarch_TagLabel->setMinimumSize(0, 0);
    qtarch_TagLabel->setMaximumSize(32767, 32767);
    qtarch_TagLabel->setFocusPolicy(QWidget::NoFocus);
    qtarch_TagLabel->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_TagLabel->setFrameStyle( 0 );
    qtarch_TagLabel->setLineWidth( 1 );
    qtarch_TagLabel->setMidLineWidth( 0 );
    qtarch_TagLabel->QFrame::setMargin( -1 );
    qtarch_TagLabel->setText( "Tag:" );
    qtarch_TagLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_TagLabel->setMargin( -1 );

    typeLabel2 = new QLabel(this, "TypeLabel2");
    typeLabel2->setGeometry(64, 5, 281, 13);
    typeLabel2->setMinimumSize(0, 0);
    typeLabel2->setMaximumSize(32767, 32767);
    typeLabel2->setFocusPolicy(QWidget::NoFocus);
    typeLabel2->setBackgroundMode(QWidget::PaletteBackground);
    typeLabel2->setFrameStyle( 0 );
    typeLabel2->setLineWidth( 1 );
    typeLabel2->setMidLineWidth( 0 );
    typeLabel2->QFrame::setMargin( -1 );
    typeLabel2->setText( "" );
    typeLabel2->setAlignment( AlignLeft|AlignVCenter|ExpandTabs );
    typeLabel2->setMargin( -1 );

    QLabel *qtarch_HLine1 = new QLabel(this, "HLine1");
    qtarch_HLine1->setGeometry(5, 23, 340, 5);
    qtarch_HLine1->setMinimumSize(0, 5);
    qtarch_HLine1->setMaximumSize(32767, 5);
    qtarch_HLine1->setFocusPolicy(QWidget::NoFocus);
    qtarch_HLine1->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_HLine1->setFrameStyle( 52 );
    qtarch_HLine1->setLineWidth( 1 );
    qtarch_HLine1->setMidLineWidth( 0 );
    qtarch_HLine1->QFrame::setMargin( -1 );
    qtarch_HLine1->setText( "" );
    qtarch_HLine1->setAlignment( AlignHCenter|AlignVCenter|ExpandTabs );
    qtarch_HLine1->setMargin( -1 );

    tagLabel = new QLabel(this, "TagLabel2");
    tagLabel->setGeometry(135, 33, 97, 59);
    tagLabel->setMinimumSize(10, 10);
    tagLabel->setMaximumSize(32767, 32767);
    tagLabel->setFocusPolicy(QWidget::NoFocus);
    tagLabel->setBackgroundMode(QWidget::PaletteBackground);
    tagLabel->setFrameStyle( 0 );
    tagLabel->setLineWidth( 1 );
    tagLabel->setMidLineWidth( 0 );
    tagLabel->QFrame::setMargin( -1 );
    tagLabel->setText( "" );
    tagLabel->setAlignment( AlignLeft|AlignVCenter|ExpandTabs );
    tagLabel->setMargin( -1 );

    tagList = new QComboBox(TRUE, this, "TagList");
    tagList->setGeometry(33, 50, 97, 25);
    tagList->setMinimumSize(10, 10);
    tagList->setMaximumSize(32767, 32767);
    tagList->setFocusPolicy(QWidget::StrongFocus);
    tagList->setBackgroundMode(QWidget::NoBackground);
    tagList->setInsertionPolicy( QComboBox::AtBottom );
    tagList->setSizeLimit( 10 );
    tagList->setAutoResize( FALSE );
    tagList->setMaxCount( 2147483647 );
    tagList->setAutoCompletion( FALSE );

    QLabel *qtarch_AddressLabel1 = new QLabel(this, "AddressLabel1");
    qtarch_AddressLabel1->setGeometry(5, 97, 88, 22);
    qtarch_AddressLabel1->setMinimumSize(0, 0);
    qtarch_AddressLabel1->setMaximumSize(32767, 32767);
    qtarch_AddressLabel1->setFocusPolicy(QWidget::NoFocus);
    qtarch_AddressLabel1->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_AddressLabel1->setFrameStyle( 0 );
    qtarch_AddressLabel1->setLineWidth( 1 );
    qtarch_AddressLabel1->setMidLineWidth( 0 );
    qtarch_AddressLabel1->QFrame::setMargin( -1 );
    qtarch_AddressLabel1->setText( "Address Line 1:" );
    qtarch_AddressLabel1->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_AddressLabel1->setMargin( -1 );

    QLabel *qtarch_AddressLabel2 = new QLabel(this, "AddressLabel2");
    qtarch_AddressLabel2->setGeometry(5, 119, 88, 22);
    qtarch_AddressLabel2->setMinimumSize(0, 0);
    qtarch_AddressLabel2->setMaximumSize(32767, 32767);
    qtarch_AddressLabel2->setFocusPolicy(QWidget::NoFocus);
    qtarch_AddressLabel2->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_AddressLabel2->setFrameStyle( 0 );
    qtarch_AddressLabel2->setLineWidth( 1 );
    qtarch_AddressLabel2->setMidLineWidth( 0 );
    qtarch_AddressLabel2->QFrame::setMargin( -1 );
    qtarch_AddressLabel2->setText( "Address Line 2:" );
    qtarch_AddressLabel2->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_AddressLabel2->setMargin( -1 );

    cszLabel = new QLabel(this, "CSZLabel");
    cszLabel->setGeometry(5, 141, 88, 22);
    cszLabel->setMinimumSize(0, 0);
    cszLabel->setMaximumSize(32767, 32767);
    cszLabel->setFocusPolicy(QWidget::NoFocus);
    cszLabel->setBackgroundMode(QWidget::PaletteBackground);
    cszLabel->setFrameStyle( 0 );
    cszLabel->setLineWidth( 1 );
    cszLabel->setMidLineWidth( 0 );
    cszLabel->QFrame::setMargin( -1 );
    cszLabel->setText( "City, State, ZIP:" );
    cszLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    cszLabel->setMargin( -1 );

    countryLabel = new QLabel(this, "CountryLabel");
    countryLabel->setGeometry(5, 185, 88, 22);
    countryLabel->setMinimumSize(0, 0);
    countryLabel->setMaximumSize(32767, 32767);
    countryLabel->setFocusPolicy(QWidget::NoFocus);
    countryLabel->setBackgroundMode(QWidget::PaletteBackground);
    countryLabel->setFrameStyle( 0 );
    countryLabel->setLineWidth( 1 );
    countryLabel->setMidLineWidth( 0 );
    countryLabel->QFrame::setMargin( -1 );
    countryLabel->setText( "Country:" );
    countryLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    countryLabel->setMargin( -1 );

    postalLabel = new QLabel(this, "PostalCodeLabel");
    postalLabel->setGeometry(5, 163, 88, 22);
    postalLabel->setMinimumSize(0, 0);
    postalLabel->setMaximumSize(32767, 32767);
    postalLabel->setFocusPolicy(QWidget::NoFocus);
    postalLabel->setBackgroundMode(QWidget::PaletteBackground);
    postalLabel->setFrameStyle( 0 );
    postalLabel->setLineWidth( 1 );
    postalLabel->setMidLineWidth( 0 );
    postalLabel->QFrame::setMargin( -1 );
    postalLabel->setText( "Postal Code:" );
    postalLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    postalLabel->setMargin( -1 );

    address1 = new QLineEdit(this, "Address1");
    address1->setGeometry(93, 97, 252, 22);
    address1->setMinimumSize(0, 0);
    address1->setMaximumSize(32767, 32767);
    address1->setFocusPolicy(QWidget::StrongFocus);
    address1->setBackgroundMode(QWidget::PaletteBase);
    address1->setText( "" );
    address1->setMaxLength( 32767 );
    address1->setFrame( QLineEdit::Normal );
    address1->setFrame( TRUE );
    address1->setAlignment( AlignLeft );

    address2 = new QLineEdit(this, "Address2");
    address2->setGeometry(93, 119, 252, 22);
    address2->setMinimumSize(0, 0);
    address2->setMaximumSize(32767, 32767);
    address2->setFocusPolicy(QWidget::StrongFocus);
    address2->setBackgroundMode(QWidget::PaletteBase);
    address2->setText( "" );
    address2->setMaxLength( 32767 );
    address2->setFrame( QLineEdit::Normal );
    address2->setFrame( TRUE );
    address2->setAlignment( AlignLeft );

    city = new QLineEdit(this, "City");
    city->setGeometry(93, 141, 142, 22);
    city->setMinimumSize(0, 0);
    city->setMaximumSize(32767, 32767);
    city->setFocusPolicy(QWidget::StrongFocus);
    city->setBackgroundMode(QWidget::PaletteBase);
    city->setText( "" );
    city->setMaxLength( 32767 );
    city->setFrame( QLineEdit::Normal );
    city->setFrame( TRUE );
    city->setAlignment( AlignLeft );

    state = new QLineEdit(this, "State");
    state->setGeometry(240, 141, 35, 22);
    state->setMinimumSize(35, 0);
    state->setMaximumSize(35, 32767);
    state->setFocusPolicy(QWidget::StrongFocus);
    state->setBackgroundMode(QWidget::PaletteBase);
    state->setText( "" );
    state->setMaxLength( 32767 );
    state->setFrame( QLineEdit::Normal );
    state->setFrame( TRUE );
    state->setAlignment( AlignLeft );

    zip = new QLineEdit(this, "ZIP");
    zip->setGeometry(280, 141, 65, 22);
    zip->setMinimumSize(65, 0);
    zip->setMaximumSize(65, 32767);
    zip->setFocusPolicy(QWidget::StrongFocus);
    zip->setBackgroundMode(QWidget::PaletteBase);
    zip->setText( "" );
    zip->setMaxLength( 32767 );
    zip->setFrame( QLineEdit::Normal );
    zip->setFrame( TRUE );
    zip->setAlignment( AlignLeft );

    postalCode = new QLineEdit(this, "PostalCode");
    postalCode->setGeometry(93, 163, 252, 22);
    postalCode->setMinimumSize(0, 0);
    postalCode->setMaximumSize(32767, 32767);
    postalCode->setFocusPolicy(QWidget::StrongFocus);
    postalCode->setBackgroundMode(QWidget::PaletteBase);
    postalCode->setText( "" );
    postalCode->setMaxLength( 32767 );
    postalCode->setFrame( QLineEdit::Normal );
    postalCode->setFrame( TRUE );
    postalCode->setAlignment( AlignLeft );

    country = new QLineEdit(this, "Country");
    country->setGeometry(93, 185, 252, 22);
    country->setMinimumSize(0, 0);
    country->setMaximumSize(32767, 32767);
    country->setFocusPolicy(QWidget::StrongFocus);
    country->setBackgroundMode(QWidget::PaletteBase);
    country->setText( "" );
    country->setMaxLength( 32767 );
    country->setFrame( QLineEdit::Normal );
    country->setFrame( TRUE );
    country->setAlignment( AlignLeft );

    QLabel *qtarch_LastModifiedLabel = new QLabel(this, "LastModifiedLabel");
    qtarch_LastModifiedLabel->setGeometry(5, 217, 77, 13);
    qtarch_LastModifiedLabel->setMinimumSize(0, 0);
    qtarch_LastModifiedLabel->setMaximumSize(32767, 32767);
    qtarch_LastModifiedLabel->setFocusPolicy(QWidget::NoFocus);
    qtarch_LastModifiedLabel->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_LastModifiedLabel->setFrameStyle( 0 );
    qtarch_LastModifiedLabel->setLineWidth( 1 );
    qtarch_LastModifiedLabel->setMidLineWidth( 0 );
    qtarch_LastModifiedLabel->QFrame::setMargin( -1 );
    qtarch_LastModifiedLabel->setText( "Last Modified:" );
    qtarch_LastModifiedLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_LastModifiedLabel->setMargin( -1 );

    lastModified = new QLabel(this, "LastModified");
    lastModified->setGeometry(87, 217, 118, 13);
    lastModified->setMinimumSize(0, 0);
    lastModified->setMaximumSize(32767, 32767);
    lastModified->setFocusPolicy(QWidget::NoFocus);
    lastModified->setBackgroundMode(QWidget::PaletteBackground);
    lastModified->setFrameStyle( 0 );
    lastModified->setLineWidth( 1 );
    lastModified->setMidLineWidth( 0 );
    lastModified->QFrame::setMargin( -1 );
    lastModified->setText( "" );
    lastModified->setAlignment( AlignLeft|AlignVCenter|ExpandTabs );
    lastModified->setMargin( -1 );

    QLabel *qtarch_LastModifiedByLabel = new QLabel(this, "LastModifiedByLabel");
    qtarch_LastModifiedByLabel->setGeometry(210, 217, 13, 13);
    qtarch_LastModifiedByLabel->setMinimumSize(0, 0);
    qtarch_LastModifiedByLabel->setMaximumSize(32767, 32767);
    qtarch_LastModifiedByLabel->setFocusPolicy(QWidget::NoFocus);
    qtarch_LastModifiedByLabel->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_LastModifiedByLabel->setFrameStyle( 0 );
    qtarch_LastModifiedByLabel->setLineWidth( 1 );
    qtarch_LastModifiedByLabel->setMidLineWidth( 0 );
    qtarch_LastModifiedByLabel->QFrame::setMargin( -1 );
    qtarch_LastModifiedByLabel->setText( "by" );
    qtarch_LastModifiedByLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_LastModifiedByLabel->setMargin( -1 );

    lastModifiedBy = new QLabel(this, "LastModifiedBy");
    lastModifiedBy->setGeometry(228, 217, 117, 13);
    lastModifiedBy->setMinimumSize(0, 0);
    lastModifiedBy->setMaximumSize(32767, 32767);
    lastModifiedBy->setFocusPolicy(QWidget::NoFocus);
    lastModifiedBy->setBackgroundMode(QWidget::PaletteBackground);
    lastModifiedBy->setFrameStyle( 0 );
    lastModifiedBy->setLineWidth( 1 );
    lastModifiedBy->setMidLineWidth( 0 );
    lastModifiedBy->QFrame::setMargin( -1 );
    lastModifiedBy->setText( "" );
    lastModifiedBy->setAlignment( AlignLeft|AlignVCenter|ExpandTabs );
    lastModifiedBy->setMargin( -1 );

    saveButton = new QPushButton(this, "SaveButton");
    saveButton->setGeometry(205, 239, 62, 26);
    saveButton->setMinimumSize(0, 0);
    saveButton->setMaximumSize(32767, 32767);
    saveButton->setFocusPolicy(QWidget::TabFocus);
    saveButton->setBackgroundMode(QWidget::PaletteButton);
    saveButton->setText( "&Save" );
    saveButton->setAutoRepeat( FALSE );
    saveButton->setAutoResize( FALSE );
    saveButton->setToggleButton( FALSE );
    saveButton->setDefault( FALSE );
    saveButton->setAutoDefault( FALSE );
    saveButton->setIsMenuButton( FALSE );
    connect(saveButton, SIGNAL(clicked()), SLOT(saveAddress()));

    canelButton = new QPushButton(this, "CancelButton");
    canelButton->setGeometry(272, 239, 73, 26);
    canelButton->setMinimumSize(0, 0);
    canelButton->setMaximumSize(32767, 32767);
    canelButton->setFocusPolicy(QWidget::TabFocus);
    canelButton->setBackgroundMode(QWidget::PaletteButton);
    canelButton->setText( "&Cancel" );
    canelButton->setAutoRepeat( FALSE );
    canelButton->setAutoResize( FALSE );
    canelButton->setToggleButton( FALSE );
    canelButton->setDefault( FALSE );
    canelButton->setAutoDefault( FALSE );
    canelButton->setIsMenuButton( FALSE );
    connect(canelButton, SIGNAL(clicked()), SLOT(cancelAddress()));

    QVButtonGroup *qtarch_TypeGroup = new QVButtonGroup(this, "TypeGroup");
    qtarch_TypeGroup->setGeometry(237, 33, 108, 59);
    qtarch_TypeGroup->setMinimumSize(0, 0);
    qtarch_TypeGroup->setMaximumSize(32767, 32767);
    qtarch_TypeGroup->setFocusPolicy(QWidget::NoFocus);
    qtarch_TypeGroup->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_TypeGroup->setFrameStyle( 0 );
    qtarch_TypeGroup->setLineWidth( 1 );
    qtarch_TypeGroup->setMidLineWidth( 0 );
    qtarch_TypeGroup->QFrame::setMargin( 0 );
    qtarch_TypeGroup->setTitle( "" );
    qtarch_TypeGroup->setAlignment( AlignLeft );
    qtarch_TypeGroup->setExclusive( FALSE );

    active = new QCheckBox(qtarch_TypeGroup, "ActiveButton");
    active->setGeometry(8, 8, 92, 19);
    active->setMinimumSize(0, 0);
    active->setMaximumSize(32767, 32767);
    active->setFocusPolicy(QWidget::TabFocus);
    active->setBackgroundMode(QWidget::PaletteBackground);
    active->setText( "Active" );
    active->setAutoRepeat( FALSE );
    active->setAutoResize( FALSE );
    active->setChecked( FALSE );

    international = new QCheckBox(qtarch_TypeGroup, "InternationalButton");
    international->setGeometry(8, 32, 92, 19);
    international->setMinimumSize(0, 0);
    international->setMaximumSize(32767, 32767);
    international->setFocusPolicy(QWidget::TabFocus);
    international->setBackgroundMode(QWidget::PaletteBackground);
    international->setText( "International" );
    international->setAutoRepeat( FALSE );
    international->setAutoResize( FALSE );
    international->setChecked( FALSE );
    connect(international, SIGNAL(toggled(bool)), SLOT(internationalChanged(bool)));

	QBoxLayout* qtarch_layout_1 = new QBoxLayout( this, QBoxLayout::TopToBottom, 5, 5, NULL );
	qtarch_layout_1->addStrut( 0 );
	QBoxLayout* qtarch_layout_1_1 = new QBoxLayout( QBoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_1, 0 );
	qtarch_layout_1_1->addStrut( 0 );
	qtarch_layout_1_1->addWidget( typeLabel, 0, 0 );
	qtarch_layout_1_1->addWidget( typeLabel2, 1, 0 );
	qtarch_layout_1->addWidget( qtarch_HLine1, 0, 0 );
	QBoxLayout* qtarch_layout_1_3 = new QBoxLayout( QBoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_3, 0 );
	qtarch_layout_1_3->addStrut( 0 );
	qtarch_layout_1_3->addWidget( qtarch_TagLabel, 0, 0 );
	qtarch_layout_1_3->addWidget( tagList, 1, 0 );
	qtarch_layout_1_3->addWidget( tagLabel, 1, 0 );
	qtarch_layout_1_3->addWidget( qtarch_TypeGroup, 0, 0 );
	QGridLayout* qtarch_layout_1_4 = new QGridLayout( 5, 2, 0, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_4, 0 );
	qtarch_layout_1_4->addColSpacing( 0, 5 );
	qtarch_layout_1_4->setColStretch( 0, 0 );
	qtarch_layout_1_4->addColSpacing( 1, 5 );
	qtarch_layout_1_4->setColStretch( 1, 1 );
	qtarch_layout_1_4->addRowSpacing( 0, 0 );
	qtarch_layout_1_4->setRowStretch( 0, 1 );
	qtarch_layout_1_4->addWidget( qtarch_AddressLabel1, 0, 0, 0 );
	qtarch_layout_1_4->addWidget( address1, 0, 1, 0 );
	qtarch_layout_1_4->addRowSpacing( 1, 0 );
	qtarch_layout_1_4->setRowStretch( 1, 1 );
	qtarch_layout_1_4->addWidget( qtarch_AddressLabel2, 1, 0, 0 );
	qtarch_layout_1_4->addWidget( address2, 1, 1, 0 );
	qtarch_layout_1_4->addRowSpacing( 2, 0 );
	qtarch_layout_1_4->setRowStretch( 2, 1 );
	qtarch_layout_1_4->addWidget( cszLabel, 2, 0, 0 );
	QBoxLayout* qtarch_layout_1_4_3_2 = new QBoxLayout( QBoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1_4->addLayout( qtarch_layout_1_4_3_2, 2, 1 );
	qtarch_layout_1_4_3_2->addStrut( 0 );
	qtarch_layout_1_4_3_2->addWidget( city, 1, 0 );
	qtarch_layout_1_4_3_2->addWidget( state, 1, 0 );
	qtarch_layout_1_4_3_2->addWidget( zip, 1, 0 );
	qtarch_layout_1_4->addRowSpacing( 3, 0 );
	qtarch_layout_1_4->setRowStretch( 3, 1 );
	qtarch_layout_1_4->addWidget( postalLabel, 3, 0, 0 );
	qtarch_layout_1_4->addWidget( postalCode, 3, 1, 0 );
	qtarch_layout_1_4->addRowSpacing( 4, 0 );
	qtarch_layout_1_4->setRowStretch( 4, 1 );
	qtarch_layout_1_4->addWidget( countryLabel, 4, 0, 0 );
	qtarch_layout_1_4->addWidget( country, 4, 1, 0 );
	qtarch_layout_1->addStretch( 1 );
	QBoxLayout* qtarch_layout_1_6 = new QBoxLayout( QBoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_6, 0 );
	qtarch_layout_1_6->addStrut( 0 );
	qtarch_layout_1_6->addWidget( qtarch_LastModifiedLabel, 0, 0 );
	qtarch_layout_1_6->addWidget( lastModified, 1, 0 );
	qtarch_layout_1_6->addWidget( qtarch_LastModifiedByLabel, 0, 0 );
	qtarch_layout_1_6->addWidget( lastModifiedBy, 1, 0 );
	qtarch_layout_1->addStretch( 1 );
	QBoxLayout* qtarch_layout_1_8 = new QBoxLayout( QBoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_8, 0 );
	qtarch_layout_1_8->addStrut( 0 );
	qtarch_layout_1_8->addStretch( 1 );
	qtarch_layout_1_8->addWidget( saveButton, 0, 0 );
	qtarch_layout_1_8->addWidget( canelButton, 0, 0 );
    resize(350,270);
    setMinimumSize(350, 270);
    setMaximumSize(32767, 32767);
}

AddressEditorData::~AddressEditorData()
{
}
