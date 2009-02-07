/***************************************************************************
** taccupgrade.h - Structure and function prototypes
****************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvatel Corporation and R. Marc Lewis.
****************************************************************************/

#ifndef TACCUPGRADE_H
#define TACCUPGRADE_H

#include <qstring.h>
#include <qptrlist.h>


int main( int argc, char ** argv );
void upgradeError(const QString &, const QString &);
void upgradeDatabase();

#endif

