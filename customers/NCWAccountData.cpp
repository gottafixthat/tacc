/**********************************************************************

	--- Qt Architect generated file ---

	File: NCWAccountData.cpp
	Last generated: Mon Mar 27 19:12:53 2000

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "NCWAccountData.h"

#include <qlabel.h>
#include <qpushbutton.h>

NCWAccountData::NCWAccountData(QWidget *parent, const char *name)
               : QWidget(parent, name, 0)
{
    helpText = new QTextView(this, "HelpText");
    helpText->setGeometry(5, 5, 175, 290);
    helpText->setMinimumSize(175, 0);
    helpText->setMaximumSize(175, 32767);
    helpText->setFocusPolicy(QWidget::NoFocus);
    helpText->setBackgroundMode(QWidget::PaletteBackground);

    QLabel *qtarch_LoginIDLabel = new QLabel(this, "LoginIDLabel");
    qtarch_LoginIDLabel->setGeometry(185, 5, 103, 25);
    qtarch_LoginIDLabel->setMinimumSize(0, 0);
    qtarch_LoginIDLabel->setMaximumSize(32767, 32767);
    qtarch_LoginIDLabel->setFocusPolicy(QWidget::NoFocus);
    qtarch_LoginIDLabel->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_LoginIDLabel->setFrameStyle( 0 );
    qtarch_LoginIDLabel->setLineWidth( 1 );
    qtarch_LoginIDLabel->setMidLineWidth( 0 );
    qtarch_LoginIDLabel->QFrame::setMargin( 0 );
    qtarch_LoginIDLabel->setText( "Login ID:" );
    qtarch_LoginIDLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_LoginIDLabel->setMargin( 0 );

    QLabel *qtarch_PasswordLabel = new QLabel(this, "PasswordLabel");
    qtarch_PasswordLabel->setGeometry(185, 32, 103, 25);
    qtarch_PasswordLabel->setMinimumSize(0, 0);
    qtarch_PasswordLabel->setMaximumSize(32767, 32767);
    qtarch_PasswordLabel->setFocusPolicy(QWidget::NoFocus);
    qtarch_PasswordLabel->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_PasswordLabel->setFrameStyle( 0 );
    qtarch_PasswordLabel->setLineWidth( 1 );
    qtarch_PasswordLabel->setMidLineWidth( 0 );
    qtarch_PasswordLabel->QFrame::setMargin( 0 );
    qtarch_PasswordLabel->setText( "Password:" );
    qtarch_PasswordLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_PasswordLabel->setMargin( 0 );

    password = new QLineEdit(this, "Password");
    password->setGeometry(290, 33, 120, 22);
    password->setMinimumSize(0, 0);
    password->setMaximumSize(32767, 32767);
    password->setFocusPolicy(QWidget::StrongFocus);
    password->setBackgroundMode(QWidget::PaletteBase);
    password->setText( "" );
    password->setMaxLength( 8 );
    password->setFrame( QLineEdit::Normal );
    password->setFrame( TRUE );
    password->setAlignment( AlignLeft );

    generateButton = new QPushButton(this, "GenerateButton");
    generateButton->setGeometry(415, 32, 80, 25);
    generateButton->setMinimumSize(80, 25);
    generateButton->setMaximumSize(80, 25);
    generateButton->setFocusPolicy(QWidget::TabFocus);
    generateButton->setBackgroundMode(QWidget::PaletteButton);
    generateButton->setText( "&Generate" );
    generateButton->setAutoRepeat( FALSE );
    generateButton->setAutoResize( FALSE );
    generateButton->setToggleButton( FALSE );
    generateButton->setDefault( FALSE );
    generateButton->setAutoDefault( TRUE );
    generateButton->setIsMenuButton( FALSE );
    connect(generateButton, SIGNAL(clicked()), SLOT(generatePassword()));

    QLabel *qtarch_LoginTypeLabel = new QLabel(this, "LoginTypeLabel");
    qtarch_LoginTypeLabel->setGeometry(185, 59, 103, 25);
    qtarch_LoginTypeLabel->setMinimumSize(0, 0);
    qtarch_LoginTypeLabel->setMaximumSize(32767, 32767);
    qtarch_LoginTypeLabel->setFocusPolicy(QWidget::NoFocus);
    qtarch_LoginTypeLabel->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_LoginTypeLabel->setFrameStyle( 0 );
    qtarch_LoginTypeLabel->setLineWidth( 1 );
    qtarch_LoginTypeLabel->setMidLineWidth( 0 );
    qtarch_LoginTypeLabel->QFrame::setMargin( 0 );
    qtarch_LoginTypeLabel->setText( "Login Type:" );
    qtarch_LoginTypeLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_LoginTypeLabel->setMargin( 0 );

    loginTypeList = new QComboBox(FALSE, this, "LoginTypeList");
    loginTypeList->setGeometry(290, 59, 205, 25);
    loginTypeList->setMinimumSize(0, 0);
    loginTypeList->setMaximumSize(32767, 32767);
    loginTypeList->setFocusPolicy(QWidget::StrongFocus);
    loginTypeList->setBackgroundMode(QWidget::NoBackground);
    loginTypeList->setSizeLimit( 10 );
    loginTypeList->setAutoResize( FALSE );
    loginTypeList->setMaxCount( 2147483647 );
    loginTypeList->setAutoCompletion( FALSE );

    QLabel *qtarch_OSLabel = new QLabel(this, "OSLabel");
    qtarch_OSLabel->setGeometry(185, 86, 103, 25);
    qtarch_OSLabel->setMinimumSize(0, 0);
    qtarch_OSLabel->setMaximumSize(32767, 32767);
    qtarch_OSLabel->setFocusPolicy(QWidget::NoFocus);
    qtarch_OSLabel->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_OSLabel->setFrameStyle( 0 );
    qtarch_OSLabel->setLineWidth( 1 );
    qtarch_OSLabel->setMidLineWidth( 0 );
    qtarch_OSLabel->QFrame::setMargin( 0 );
    qtarch_OSLabel->setText( "Operating System:" );
    qtarch_OSLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_OSLabel->setMargin( 0 );

    OSList = new QComboBox(FALSE, this, "OSList");
    OSList->setGeometry(290, 86, 205, 25);
    OSList->setMinimumSize(0, 0);
    OSList->setMaximumSize(32767, 32767);
    OSList->setFocusPolicy(QWidget::StrongFocus);
    OSList->setBackgroundMode(QWidget::NoBackground);
    OSList->setSizeLimit( 10 );
    OSList->setAutoResize( FALSE );
    OSList->insertItem( "Windows 9x" );
    OSList->insertItem( "Mac OS 8+" );
    OSList->insertItem( "Windows NT/2000" );
    OSList->insertItem( "Linux" );
    OSList->insertItem( "Mac OS 7.x" );
    OSList->insertItem( "Windows 3.1" );
    OSList->insertItem( "Other" );
    OSList->setMaxCount( 2147483647 );
    OSList->setAutoCompletion( FALSE );

    QLabel *qtarch_PackageLabel = new QLabel(this, "PackageLabel");
    qtarch_PackageLabel->setGeometry(185, 113, 103, 25);
    qtarch_PackageLabel->setMinimumSize(0, 0);
    qtarch_PackageLabel->setMaximumSize(32767, 32767);
    qtarch_PackageLabel->setFocusPolicy(QWidget::NoFocus);
    qtarch_PackageLabel->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_PackageLabel->setFrameStyle( 0 );
    qtarch_PackageLabel->setLineWidth( 1 );
    qtarch_PackageLabel->setMidLineWidth( 0 );
    qtarch_PackageLabel->QFrame::setMargin( 0 );
    qtarch_PackageLabel->setText( "Package:" );
    qtarch_PackageLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_PackageLabel->setMargin( 0 );

    packageList = new QComboBox(FALSE, this, "PackageList");
    packageList->setGeometry(290, 113, 205, 25);
    packageList->setMinimumSize(0, 0);
    packageList->setMaximumSize(32767, 32767);
    packageList->setFocusPolicy(QWidget::StrongFocus);
    packageList->setBackgroundMode(QWidget::NoBackground);
    packageList->setSizeLimit( 10 );
    packageList->setAutoResize( FALSE );
    packageList->setMaxCount( 2147483647 );
    packageList->setAutoCompletion( FALSE );

    QLabel *qtarch_RatePlanLabel = new QLabel(this, "RatePlanLabel");
    qtarch_RatePlanLabel->setGeometry(185, 140, 103, 25);
    qtarch_RatePlanLabel->setMinimumSize(0, 0);
    qtarch_RatePlanLabel->setMaximumSize(32767, 32767);
    qtarch_RatePlanLabel->setFocusPolicy(QWidget::NoFocus);
    qtarch_RatePlanLabel->setBackgroundMode(QWidget::PaletteBackground);
    qtarch_RatePlanLabel->setFrameStyle( 0 );
    qtarch_RatePlanLabel->setLineWidth( 1 );
    qtarch_RatePlanLabel->setMidLineWidth( 0 );
    qtarch_RatePlanLabel->QFrame::setMargin( 0 );
    qtarch_RatePlanLabel->setText( "Rate Plan:" );
    qtarch_RatePlanLabel->setAlignment( AlignRight|AlignVCenter|ExpandTabs );
    qtarch_RatePlanLabel->setMargin( 0 );

    ratePlanList = new QComboBox(FALSE, this, "RatePlanList");
    ratePlanList->setGeometry(290, 140, 205, 25);
    ratePlanList->setMinimumSize(0, 0);
    ratePlanList->setMaximumSize(32767, 32767);
    ratePlanList->setFocusPolicy(QWidget::StrongFocus);
    ratePlanList->setBackgroundMode(QWidget::NoBackground);
    ratePlanList->setSizeLimit( 10 );
    ratePlanList->setAutoResize( FALSE );
    ratePlanList->setMaxCount( 2147483647 );
    ratePlanList->setAutoCompletion( FALSE );

    QPushButton *qtarch_SuggestButton = new QPushButton(this, "SuggestButton");
    qtarch_SuggestButton->setGeometry(415, 5, 80, 25);
    qtarch_SuggestButton->setMinimumSize(80, 25);
    qtarch_SuggestButton->setMaximumSize(80, 25);
    qtarch_SuggestButton->setFocusPolicy(QWidget::TabFocus);
    qtarch_SuggestButton->setBackgroundMode(QWidget::PaletteButton);
    qtarch_SuggestButton->setText( "S&uggest" );
    qtarch_SuggestButton->setAutoRepeat( FALSE );
    qtarch_SuggestButton->setAutoResize( FALSE );
    qtarch_SuggestButton->setToggleButton( FALSE );
    qtarch_SuggestButton->setDefault( FALSE );
    qtarch_SuggestButton->setAutoDefault( TRUE );
    qtarch_SuggestButton->setIsMenuButton( FALSE );

    loginID = new QLineEdit(this, "LoginID");
    loginID->setGeometry(290, 6, 120, 22);
    loginID->setMinimumSize(0, 0);
    loginID->setMaximumSize(32767, 32767);
    loginID->setFocusPolicy(QWidget::StrongFocus);
    loginID->setBackgroundMode(QWidget::PaletteBase);
    loginID->setText( "" );
    loginID->setMaxLength( 8 );
    loginID->setFrame( QLineEdit::Normal );
    loginID->setFrame( TRUE );
    loginID->setAlignment( AlignLeft );

	QBoxLayout* qtarch_layout_1 = new QBoxLayout( this, QBoxLayout::LeftToRight, 5, 5, NULL );
	qtarch_layout_1->addStrut( 0 );
	qtarch_layout_1->addWidget( helpText, 1, 0 );
	QBoxLayout* qtarch_layout_1_2 = new QBoxLayout( QBoxLayout::TopToBottom, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_2, 1 );
	qtarch_layout_1_2->addStrut( 0 );
	QGridLayout* qtarch_layout_1_2_1 = new QGridLayout( 6, 2, 2, NULL );
	qtarch_layout_1_2->addLayout( qtarch_layout_1_2_1, 0 );
	qtarch_layout_1_2_1->addColSpacing( 0, 5 );
	qtarch_layout_1_2_1->setColStretch( 0, 0 );
	qtarch_layout_1_2_1->addColSpacing( 1, 5 );
	qtarch_layout_1_2_1->setColStretch( 1, 1 );
	qtarch_layout_1_2_1->addRowSpacing( 0, 0 );
	qtarch_layout_1_2_1->setRowStretch( 0, 1 );
	qtarch_layout_1_2_1->addWidget( qtarch_LoginIDLabel, 0, 0, 0 );
	QBoxLayout* qtarch_layout_1_2_1_1_2 = new QBoxLayout( QBoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1_2_1->addLayout( qtarch_layout_1_2_1_1_2, 0, 1 );
	qtarch_layout_1_2_1_1_2->addStrut( 0 );
	qtarch_layout_1_2_1_1_2->addWidget( loginID, 1, 0 );
	qtarch_layout_1_2_1_1_2->addWidget( qtarch_SuggestButton, 0, 0 );
	qtarch_layout_1_2_1->addRowSpacing( 1, 0 );
	qtarch_layout_1_2_1->setRowStretch( 1, 1 );
	qtarch_layout_1_2_1->addWidget( qtarch_PasswordLabel, 1, 0, 0 );
	QBoxLayout* qtarch_layout_1_2_1_2_2 = new QBoxLayout( QBoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1_2_1->addLayout( qtarch_layout_1_2_1_2_2, 1, 1 );
	qtarch_layout_1_2_1_2_2->addStrut( 0 );
	qtarch_layout_1_2_1_2_2->addWidget( password, 1, 0 );
	qtarch_layout_1_2_1_2_2->addWidget( generateButton, 0, 0 );
	qtarch_layout_1_2_1->addRowSpacing( 2, 0 );
	qtarch_layout_1_2_1->setRowStretch( 2, 1 );
	qtarch_layout_1_2_1->addWidget( qtarch_LoginTypeLabel, 2, 0, 0 );
	qtarch_layout_1_2_1->addWidget( loginTypeList, 2, 1, 0 );
	qtarch_layout_1_2_1->addRowSpacing( 3, 0 );
	qtarch_layout_1_2_1->setRowStretch( 3, 1 );
	qtarch_layout_1_2_1->addWidget( qtarch_OSLabel, 3, 0, 0 );
	qtarch_layout_1_2_1->addWidget( OSList, 3, 1, 0 );
	qtarch_layout_1_2_1->addRowSpacing( 4, 0 );
	qtarch_layout_1_2_1->setRowStretch( 4, 1 );
	qtarch_layout_1_2_1->addWidget( qtarch_PackageLabel, 4, 0, 0 );
	qtarch_layout_1_2_1->addWidget( packageList, 4, 1, 0 );
	qtarch_layout_1_2_1->addRowSpacing( 5, 0 );
	qtarch_layout_1_2_1->setRowStretch( 5, 1 );
	qtarch_layout_1_2_1->addWidget( qtarch_RatePlanLabel, 5, 0, 0 );
	qtarch_layout_1_2_1->addWidget( ratePlanList, 5, 1, 0 );
	qtarch_layout_1_2->addStretch( 1 );
    resize(500,300);
    setMinimumSize(0, 0);
    setMaximumSize(32767, 32767);
}

NCWAccountData::~NCWAccountData()
{
}
