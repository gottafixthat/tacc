/**********************************************************************

    RunStatements.h

 *********************************************************************/

#ifndef RunStatements_included
#define RunStatements_included


void RunStatements(void);
int  doStatement(long CustID);
void doAutoPayments(long CustID);
void emailStatement(long StNo, int Force, int Dup);
void printStatement(long StNo, int Force, int Dup);
float getAutoPaymentLine(long CustID, char *);

#endif // RunSubscriptions_included
