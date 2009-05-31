/**************************************************************************
**
** cdrs.h - Main program file and class definitions.
**
***************************************************************************
** Written by R. Marc Lewis <marc@avvatel.com>
**   (C)opyright 2008, R. Marc Lewis
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of R. Marc Lewis or Avvatel Corporation.
***************************************************************************/


#ifndef CDRS_H
#define CDRS_H

#include <QtCore/QCoreApplication>

class cdrsApp : public QCoreApplication
{
    Q_OBJECT

public:
    cdrsApp(int &, char **);

};

int main(int, char**);
void showHelp(void);

#endif
