/*
** Blarg DES program key routines.
*/

#ifndef LOADKEY_H
#define LOADKEY_H

typedef struct ProgramKeyStruct {
    char    CompanyName[32];
    char    CompanyAddr1[32];
    char    CompanyAddr2[32];
    char    CompanyCity[32];
    char    CompanyState[32];
    char    CompanyPostal[32];
    char    CompanyCountry[32];
    char    CompanyPhone[32];
    char    CompanyFax[32];
    char    PurchaseDate[32];
    char    ExpDate[32];
    long    MaxUsers;
    char    CipherKey[32];
};


void loadKey(void);

#endif
