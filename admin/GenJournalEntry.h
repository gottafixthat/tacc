/**********************************************************************

	--- Dlgedit generated file ---

	File: GenJournalEntry.h
	Last generated: Sun Oct 26 12:29:47 1997

 *********************************************************************/

#ifndef GenJournalEntry_included
#define GenJournalEntry_included

#include "GenJournalEntryData.h"

#include <qcombo.h>
#include <qlined.h>

#define GJELINES 8

class GenJournalEntry : public GenJournalEntryData
{
    Q_OBJECT

public:

    GenJournalEntry
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~GenJournalEntry();

protected slots:
	virtual void saveEntry();
	virtual void cancelEntry();

private:
	int validateEntry();
	
	QComboBox * accounts[GJELINES];
	QLineEdit * credits[GJELINES];
	QLineEdit * debits[GJELINES];
	QLineEdit * memos[GJELINES];
	
	int	*acctNoIndex;

};
#endif // GenJournalEntry_included
