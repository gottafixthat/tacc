/**
 * LoginService.h - Class for handling "Login" types, which are the
 * standard Blarg/Avvanta login types.
 *
 * For now, this will only handle the BRASS interactions.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef LOGINSERVICE_H
#define LOGINSERVICE_H

#include <BRASSService.h>

class LoginService : public BRASSService
{

public:
    LoginService();
    virtual ~LoginService();

    virtual int         loadFlags();

};


#endif

