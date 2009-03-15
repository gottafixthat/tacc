/**
 * BRASSService.h - An intermediary class for BRASS based services.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef BRASSSERVICE_H
#define BRASSSERVICE_H

#include <BrassClient.h>
#include <TAAService.h>

class BRASSService : public TAAService
{

public:
    BRASSService();
    virtual ~BRASSService();

    virtual int provision();

    virtual int loginExists();
    virtual int createLogin(QString loginID);
    virtual int setPassword(QString loginID, QString pass);
    virtual int setFingerName(QString loginID, QString gecos);

protected:
    int             brassConnect();
    BrassClient     *BC;
};


#endif

