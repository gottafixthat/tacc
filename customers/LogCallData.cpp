/**********************************************************************

	--- Qt Architect generated file ---

	File: LogCallData.cpp
	Last generated: Mon Apr 24 11:13:44 2000

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3BoxLayout>
#include <Q3GridLayout>
#include <Q3Frame>
#include <QLabel>
#include "LogCallData.h"

#include <qpushbutton.h>

LogCallData::LogCallData(QWidget *parent, const char *name)
            : QWidget(parent, name, 0)
{
    loginIDLabel = new QLabel(this, "LoginIDLabel");
    loginIDLabel->setGeometry(5, 5, 75, 25);
    loginIDLabel->setMinimumSize(75, 25);
    loginIDLabel->setMaximumSize(75, 25);
    loginIDLabel->setFocusPolicy(Qt::NoFocus);
    loginIDLabel->setBackgroundMode(Qt::PaletteBackground);
    loginIDLabel->setFontPropagation(QWidget::SameFont);
    loginIDLabel->setPalettePropagation(QWidget::SameFont);
    loginIDLabel->setFrameStyle( 0 );
    loginIDLabel->setLineWidth( 1 );
    loginIDLabel->setMidLineWidth( 0 );
    loginIDLabel->Q3Frame::setMargin( -1 );
    loginIDLabel->setText( "&Login ID:" );
    loginIDLabel->setAlignment( Qt::AlignRight|Qt::AlignVCenter|Qt::TextExpandTabs );
    loginIDLabel->setMargin( -1 );

    loginID = new QLineEdit(this, "LoginIDEntry");
    loginID->setGeometry(85, 7, 130, 22);
    loginID->setMinimumSize(0, 0);
    loginID->setMaximumSize(32767, 32767);
    loginID->setFocusPolicy(Qt::StrongFocus);
    loginID->setBackgroundMode(Qt::PaletteBase);
    loginID->setFontPropagation(QWidget::SameFont);
    loginID->setPalettePropagation(QWidget::SameFont);
    loginID->setText( "" );
    loginID->setMaxLength( 16 );
    loginID->setFrame( QLineEdit::Normal );
    loginID->setFrame( TRUE );
    loginID->setAlignment( Qt::AlignLeft );
    connect(loginID, SIGNAL(textChanged(const QString&)), SLOT(loginIDChanged(const QString&)));

    searchButton = new QPushButton(this, "SearchButton");
    searchButton->setGeometry(220, 5, 72, 26);
    searchButton->setMinimumSize(0, 0);
    searchButton->setMaximumSize(32767, 32767);
    searchButton->setFocusPolicy(Qt::TabFocus);
    searchButton->setBackgroundMode(Qt::PaletteBackground);
    searchButton->setFontPropagation(QWidget::SameFont);
    searchButton->setPalettePropagation(QWidget::SameFont);
    searchButton->setText( "&Search" );
    searchButton->setAutoRepeat( FALSE );
    searchButton->setAutoResize( FALSE );
    searchButton->setToggleButton( FALSE );
    searchButton->setDefault( FALSE );
    searchButton->setAutoDefault( FALSE );
    searchButton->setIsMenuButton( FALSE );
    connect(searchButton, SIGNAL(clicked()), SLOT(findLogin()));

    typeLabel = new QLabel(this, "TypeLabel");
    typeLabel->setGeometry(297, 5, 38, 26);
    typeLabel->setMinimumSize(0, 0);
    typeLabel->setMaximumSize(32767, 32767);
    typeLabel->setFocusPolicy(Qt::NoFocus);
    typeLabel->setBackgroundMode(Qt::PaletteBackground);
    typeLabel->setFontPropagation(QWidget::SameFont);
    typeLabel->setPalettePropagation(QWidget::SameFont);
    typeLabel->setFrameStyle( 0 );
    typeLabel->setLineWidth( 1 );
    typeLabel->setMidLineWidth( 0 );
    typeLabel->Q3Frame::setMargin( -1 );
    typeLabel->setText( "&Type:" );
    typeLabel->setAlignment( Qt::AlignRight|Qt::AlignVCenter|Qt::TextExpandTabs );
    typeLabel->setMargin( -1 );

    callType = new QComboBox(FALSE, this, "CallType");
    callType->setGeometry(340, 7, 130, 22);
    callType->setMinimumSize(0, 0);
    callType->setMaximumSize(32767, 32767);
    callType->setFocusPolicy(Qt::StrongFocus);
    callType->setBackgroundMode(Qt::PaletteBackground);
    callType->setFontPropagation(QWidget::SameFont);
    callType->setPalettePropagation(QWidget::SameFont);
    callType->setSizeLimit( 10 );
    callType->setAutoResize( FALSE );
    callType->insertItem( "Support" );
    callType->insertItem( "Sales" );
    callType->insertItem( "New User" );
    callType->insertItem( "Accounting" );
    callType->setMaxCount( 2147483647 );
    callType->setAutoCompletion( FALSE );
    connect(callType, SIGNAL(highlighted(int)), SLOT(callTypeChanged(int)));

    reasonLabel = new QLabel(this, "ReasonLabel");
    reasonLabel->setGeometry(5, 33, 75, 84);
    reasonLabel->setMinimumSize(75, 0);
    reasonLabel->setMaximumSize(75, 32767);
    reasonLabel->setFocusPolicy(Qt::NoFocus);
    reasonLabel->setBackgroundMode(Qt::PaletteBackground);
    reasonLabel->setFontPropagation(QWidget::SameFont);
    reasonLabel->setPalettePropagation(QWidget::SameFont);
    reasonLabel->setFrameStyle( 0 );
    reasonLabel->setLineWidth( 1 );
    reasonLabel->setMidLineWidth( 0 );
    reasonLabel->Q3Frame::setMargin( -1 );
    reasonLabel->setText( "&Reason:" );
    reasonLabel->setAlignment( Qt::AlignRight|Qt::AlignTop|Qt::TextExpandTabs );
    reasonLabel->setMargin( -1 );

    reasonList = new Q3ListView(this, "ReasonList");
    reasonList->setGeometry(85, 33, 385, 84);
    reasonList->setMinimumSize(0, 84);
    reasonList->setMaximumSize(32767, 84);
    reasonList->setFocusPolicy(Qt::TabFocus);
    reasonList->setBackgroundMode(Qt::PaletteBackground);
    reasonList->setFontPropagation(QWidget::SameFont);
    reasonList->setPalettePropagation(QWidget::SameFont);
    reasonList->setFrameStyle( 17 );
    reasonList->setLineWidth( 2 );
    reasonList->setMidLineWidth( 0 );
    reasonList->Q3Frame::setMargin( 0 );
    reasonList->setResizePolicy( Q3ScrollView::Manual );
    reasonList->setVScrollBarMode( Q3ScrollView::Auto );
    reasonList->setHScrollBarMode( Q3ScrollView::Auto );
    reasonList->setTreeStepSize( 20 );
    reasonList->setMultiSelection( FALSE );
    reasonList->setAllColumnsShowFocus( FALSE );
    reasonList->setItemMargin( 1 );
    reasonList->setRootIsDecorated( FALSE );
    reasonList->addColumn( "Sample items", -1 );
    reasonList->setColumnWidthMode( 0, Q3ListView::Maximum );
    reasonList->setColumnAlignment( 0, 1 );

    solutionLabel = new QLabel(this, "SolutionLabel");
    solutionLabel->setGeometry(5, 119, 75, 120);
    solutionLabel->setMinimumSize(75, 0);
    solutionLabel->setMaximumSize(75, 32767);
    solutionLabel->setFocusPolicy(Qt::NoFocus);
    solutionLabel->setBackgroundMode(Qt::PaletteBackground);
    solutionLabel->setFontPropagation(QWidget::SameFont);
    solutionLabel->setPalettePropagation(QWidget::SameFont);
    solutionLabel->setFrameStyle( 0 );
    solutionLabel->setLineWidth( 1 );
    solutionLabel->setMidLineWidth( 0 );
    solutionLabel->Q3Frame::setMargin( -1 );
    solutionLabel->setText( "S&olution:" );
    solutionLabel->setAlignment( Qt::AlignRight|Qt::AlignTop|Qt::TextExpandTabs );
    solutionLabel->setMargin( -1 );

    solutionNotes = new Q3MultiLineEdit(this, "SolutionNotes");
    solutionNotes->setGeometry(85, 119, 385, 120);
    solutionNotes->setMinimumSize(0, 0);
    solutionNotes->setMaximumSize(32767, 32767);
    solutionNotes->setFocusPolicy(Qt::StrongFocus);
    solutionNotes->setBackgroundMode(Qt::PaletteBase);
    solutionNotes->setFontPropagation(QWidget::SameFont);
    solutionNotes->setPalettePropagation(QWidget::SameFont);
    solutionNotes->setFrameStyle( 51 );
    solutionNotes->setLineWidth( 2 );
    solutionNotes->setMidLineWidth( 0 );
    solutionNotes->Q3Frame::setMargin( 0 );
    solutionNotes->insertLine( "" );
    solutionNotes->setReadOnly( FALSE );
    solutionNotes->setOverwriteMode( FALSE );
    solutionNotes->setAutoUpdate( TRUE );

    appendButton = new QPushButton(this, "AddToCallButton");
    appendButton->setGeometry(162, 241, 151, 26);
    appendButton->setMinimumSize(0, 0);
    appendButton->setMaximumSize(32767, 32767);
    appendButton->setFocusPolicy(Qt::TabFocus);
    appendButton->setBackgroundMode(Qt::PaletteBackground);
    appendButton->setFontPropagation(QWidget::SameFont);
    appendButton->setPalettePropagation(QWidget::SameFont);
    appendButton->setText( "&Add to prev call" );
    appendButton->setAutoRepeat( FALSE );
    appendButton->setAutoResize( FALSE );
    appendButton->setToggleButton( FALSE );
    appendButton->setDefault( FALSE );
    appendButton->setAutoDefault( FALSE );
    appendButton->setIsMenuButton( FALSE );
    connect(appendButton, SIGNAL(clicked()), SLOT(addToCall()));

    clockButton = new QPushButton(this, "ClockButton");
    clockButton->setGeometry(318, 241, 152, 26);
    clockButton->setMinimumSize(0, 0);
    clockButton->setMaximumSize(32767, 32767);
    clockButton->setFocusPolicy(Qt::TabFocus);
    clockButton->setBackgroundMode(Qt::PaletteBackground);
    clockButton->setFontPropagation(QWidget::SameFont);
    clockButton->setPalettePropagation(QWidget::SameFont);
    clockButton->setText( "&Stop Timer" );
    clockButton->setAutoRepeat( FALSE );
    clockButton->setAutoResize( FALSE );
    clockButton->setToggleButton( FALSE );
    clockButton->setDefault( FALSE );
    clockButton->setAutoDefault( FALSE );
    clockButton->setIsMenuButton( FALSE );
    connect(clockButton, SIGNAL(clicked()), SLOT(toggleTimer()));

    viewLogButton = new QPushButton(this, "ViewLogButton");
    viewLogButton->setGeometry(5, 272, 152, 26);
    viewLogButton->setMinimumSize(0, 0);
    viewLogButton->setMaximumSize(32767, 32767);
    viewLogButton->setFocusPolicy(Qt::TabFocus);
    viewLogButton->setBackgroundMode(Qt::PaletteBackground);
    viewLogButton->setFontPropagation(QWidget::SameFont);
    viewLogButton->setPalettePropagation(QWidget::SameFont);
    viewLogButton->setText( "&View Log" );
    viewLogButton->setAutoRepeat( FALSE );
    viewLogButton->setAutoResize( FALSE );
    viewLogButton->setToggleButton( FALSE );
    viewLogButton->setDefault( FALSE );
    viewLogButton->setAutoDefault( FALSE );
    viewLogButton->setIsMenuButton( FALSE );
    connect(viewLogButton, SIGNAL(clicked()), SLOT(viewCallLog()));

    QPushButton *qtarch_LoadCustButton = new QPushButton(this, "LoadCustButton");
    qtarch_LoadCustButton->setGeometry(162, 272, 151, 26);
    qtarch_LoadCustButton->setMinimumSize(0, 0);
    qtarch_LoadCustButton->setMaximumSize(32767, 32767);
    qtarch_LoadCustButton->setFocusPolicy(Qt::TabFocus);
    qtarch_LoadCustButton->setBackgroundMode(Qt::PaletteBackground);
    qtarch_LoadCustButton->setFontPropagation(QWidget::SameFont);
    qtarch_LoadCustButton->setPalettePropagation(QWidget::SameFont);
    qtarch_LoadCustButton->setText( "Open &Cust" );
    qtarch_LoadCustButton->setAutoRepeat( FALSE );
    qtarch_LoadCustButton->setAutoResize( FALSE );
    qtarch_LoadCustButton->setToggleButton( FALSE );
    qtarch_LoadCustButton->setDefault( FALSE );
    qtarch_LoadCustButton->setAutoDefault( FALSE );
    qtarch_LoadCustButton->setIsMenuButton( FALSE );
    connect(qtarch_LoadCustButton, SIGNAL(clicked()), SLOT(loadCustomer()));

    doneButton = new QPushButton(this, "DoneButton");
    doneButton->setGeometry(318, 272, 152, 26);
    doneButton->setMinimumSize(0, 0);
    doneButton->setMaximumSize(32767, 32767);
    doneButton->setFocusPolicy(Qt::TabFocus);
    doneButton->setBackgroundMode(Qt::PaletteBackground);
    doneButton->setFontPropagation(QWidget::SameFont);
    doneButton->setPalettePropagation(QWidget::SameFont);
    doneButton->setText( "&Done" );
    doneButton->setAutoRepeat( FALSE );
    doneButton->setAutoResize( FALSE );
    doneButton->setToggleButton( FALSE );
    doneButton->setDefault( FALSE );
    doneButton->setAutoDefault( FALSE );
    doneButton->setIsMenuButton( FALSE );
    connect(doneButton, SIGNAL(clicked()), SLOT(hangupCall()));

    statusLabel = new QLabel(this, "StatusLabel");
    statusLabel->setGeometry(5, 300, 378, 25);
    statusLabel->setMinimumSize(0, 25);
    statusLabel->setMaximumSize(32767, 5);
    statusLabel->setFocusPolicy(Qt::NoFocus);
    statusLabel->setBackgroundMode(Qt::PaletteBackground);
    statusLabel->setFontPropagation(QWidget::SameFont);
    statusLabel->setPalettePropagation(QWidget::SameFont);
    statusLabel->setFrameStyle( 51 );
    statusLabel->setLineWidth( 2 );
    statusLabel->setMidLineWidth( 0 );
    statusLabel->Q3Frame::setMargin( -1 );
    statusLabel->setText( "Status Label" );
    statusLabel->setAlignment( Qt::AlignLeft|Qt::AlignVCenter|Qt::TextExpandTabs );
    statusLabel->setMargin( -1 );

    clockArea = new QLCDNumber(this, "LCDNumber_1");
    clockArea->setGeometry(388, 300, 82, 25);
    clockArea->setMinimumSize(0, 0);
    clockArea->setMaximumSize(32767, 32767);
    clockArea->setFocusPolicy(Qt::NoFocus);
    clockArea->setBackgroundMode(Qt::PaletteBackground);
    clockArea->setFontPropagation(QWidget::SameFont);
    clockArea->setPalettePropagation(QWidget::SameFont);
    clockArea->setFrameStyle( 33 );
    clockArea->setLineWidth( 1 );
    clockArea->setMidLineWidth( 0 );
    clockArea->Q3Frame::setMargin( 0 );
    clockArea->setSmallDecimalPoint( FALSE );
    clockArea->setNumDigits( 7 );
    clockArea->setMode( QLCDNumber::DEC );
    clockArea->setSegmentStyle( QLCDNumber::Flat );

	Q3BoxLayout* qtarch_layout_1 = new Q3BoxLayout( this, Q3BoxLayout::TopToBottom, 5, 2, NULL );
	qtarch_layout_1->addStrut( 0 );
	Q3GridLayout* qtarch_layout_1_1 = new Q3GridLayout( 1, 5, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_1, 0 );
	qtarch_layout_1_1->addColSpacing( 0, 5 );
	qtarch_layout_1_1->setColStretch( 0, 0 );
	qtarch_layout_1_1->addColSpacing( 1, 5 );
	qtarch_layout_1_1->setColStretch( 1, 1 );
	qtarch_layout_1_1->addColSpacing( 2, 5 );
	qtarch_layout_1_1->setColStretch( 2, 0 );
	qtarch_layout_1_1->addColSpacing( 3, 5 );
	qtarch_layout_1_1->setColStretch( 3, 0 );
	qtarch_layout_1_1->addColSpacing( 4, 5 );
	qtarch_layout_1_1->setColStretch( 4, 1 );
	qtarch_layout_1_1->addRowSpacing( 0, 0 );
	qtarch_layout_1_1->setRowStretch( 0, 1 );
	qtarch_layout_1_1->addWidget( loginIDLabel, 0, 0, 0 );
	qtarch_layout_1_1->addWidget( loginID, 0, 1, 0 );
	qtarch_layout_1_1->addWidget( searchButton, 0, 2, 0 );
	qtarch_layout_1_1->addWidget( typeLabel, 0, 3, 0 );
	qtarch_layout_1_1->addWidget( callType, 0, 4, 0 );
	Q3BoxLayout* qtarch_layout_1_2 = new Q3BoxLayout( Q3BoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_2, 1 );
	qtarch_layout_1_2->addStrut( 0 );
	qtarch_layout_1_2->addWidget( reasonLabel, 1, 0 );
	qtarch_layout_1_2->addWidget( reasonList, 1, 0 );
	Q3BoxLayout* qtarch_layout_1_3 = new Q3BoxLayout( Q3BoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_3, 1 );
	qtarch_layout_1_3->addStrut( 0 );
	qtarch_layout_1_3->addWidget( solutionLabel, 1, 0 );
	qtarch_layout_1_3->addWidget( solutionNotes, 1, 0 );
	Q3GridLayout* qtarch_layout_1_4 = new Q3GridLayout( 2, 3, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_4, 1 );
	qtarch_layout_1_4->addColSpacing( 0, 5 );
	qtarch_layout_1_4->setColStretch( 0, 1 );
	qtarch_layout_1_4->addColSpacing( 1, 5 );
	qtarch_layout_1_4->setColStretch( 1, 1 );
	qtarch_layout_1_4->addColSpacing( 2, 5 );
	qtarch_layout_1_4->setColStretch( 2, 1 );
	qtarch_layout_1_4->addRowSpacing( 0, 0 );
	qtarch_layout_1_4->setRowStretch( 0, 1 );
	qtarch_layout_1_4->addWidget( appendButton, 0, 1, 0 );
	qtarch_layout_1_4->addWidget( clockButton, 0, 2, 0 );
	qtarch_layout_1_4->addRowSpacing( 1, 0 );
	qtarch_layout_1_4->setRowStretch( 1, 1 );
	qtarch_layout_1_4->addWidget( viewLogButton, 1, 0, 0 );
	qtarch_layout_1_4->addWidget( qtarch_LoadCustButton, 1, 1, 0 );
	qtarch_layout_1_4->addWidget( doneButton, 1, 2, 0 );
	Q3BoxLayout* qtarch_layout_1_5 = new Q3BoxLayout( Q3BoxLayout::LeftToRight, 5, NULL );
	qtarch_layout_1->addLayout( qtarch_layout_1_5, 1 );
	qtarch_layout_1_5->addStrut( 0 );
	qtarch_layout_1_5->addWidget( statusLabel, 1, 0 );
	qtarch_layout_1_5->addWidget( clockArea, 0, 0 );
    resize(475,330);
    setMinimumSize(500, 100);
    setMaximumSize(32767, 32767);
}

LogCallData::~LogCallData()
{
}
