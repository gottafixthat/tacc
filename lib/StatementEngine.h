/**
 * StatementEngine.h - Definition for the statement engine class.
 *
 * Initially this class will just consolodate the statement functions
 * that are scattered around through various other source files.
 * Eventually it should be transformed into a full-fledged engine for
 * processing customer statements.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef STATEMENTENGINE_H
#define STATEMENTENGINE_H

#include <qcstring.h>
#include <wtpl2.h>

class StatementEngine
{
public:
    StatementEngine();
    ~StatementEngine();

    void        runStatements(void);
    int         doStatement(uint CustID);
    void        doAutoPayments(uint CustID);
    void        emailStatement(uint StNo, int Force, int Dup);
    void        printStatement(uint StNo, int Force, int Dup);
    float       getAutoPaymentLine(uint CustID, char *);

    void        emailLatexStatement(uint StNo);
    void        generateLatexStatement(uint statementNo, int printIt = 0);
    wtpl        *parseStatementTemplate(uint statementNo, const char *fileName);


protected:
    void        clearOutputs();
    uint        currentStatementNo;
    QByteArray  textOutput;
    QByteArray  pdfOutput;
    QByteArray  psOutput;
};

#endif
