/**********************************************************************

	--- Qt Architect generated file ---

	File: NCWAccount.h
	Last generated: Mon Mar 27 17:59:54 2000

 *********************************************************************/

#ifndef NCWAccount_included
#define NCWAccount_included

#include "NCWAccountData.h"

class NCWAccount : public NCWAccountData
{
    Q_OBJECT

public:

    NCWAccount(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~NCWAccount();

    protected slots:

    virtual void generatePassword();
};
#endif // NCWAccount_included
