/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 *
 * cmrimport - ChoiceMax Rates Import program for importing PointOne's 
 * choicemax rates into the AVP database for use with TACC's VoIP Rating
 * system.
 */

#ifndef CMRIMPORT_H
#define CMRIMPORT_H

#include <QtCore/QString>

struct tfrateRecord {
    QString LATA;
    QString LATAName;
    QString Rate;
};

int main(int argc, char **argv);
void listCarriers(void);
bool readFile(QString);
bool checkCarrierID(void);
void insertRates(void);

#endif
// vim: expandtab
