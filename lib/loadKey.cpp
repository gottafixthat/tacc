#include <loadKey.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <qdatetm.h>
#include <bdes.h>

#define TACT_KEYFILE "/usr/local/lib/tact.key"

static struct ProgramKeyStruct ProgramKey;

/*
** loadKey - Loads the program key and stuffs it into memory.
**           The key must be called '/usr/local/lib/tact.key'
**           It also sets up the user key which is used by the
**           encrypt_string and decrypt_string functions.
**           If the key is invalid, it aborts the program.
*/

void loadKey(void)
{
    //set_user_key((unsigned char *) "213434ak");
    //return;
    FILE        *fp;
    int         Success = 1;
    int         LineNo  = 0;
    unsigned    char *src;
    unsigned    char *dst;
    extern      ProgramKeyStruct ProgramKey;
    // extern      unsigned char cbc_user[8];

    // Clear out the program key information.
    memset(&ProgramKey, 0, sizeof(ProgramKey));
    
    src = (unsigned char *) calloc(1024, sizeof(char));
    dst = (unsigned char *) calloc(1024, sizeof(char));
    
    fp = fopen(TACT_KEYFILE, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to load key file '%s'.  Exiting.\n", TACT_KEYFILE);
        exit(-1);
    }

    while (fgets((char *)src, 1000, fp) != NULL) {
        LineNo++;
        if (Success) {
            if (!decrypt_string(src, dst, 0)) {
                //fprintf(stderr, "Line %d = '%s'\n", LineNo, dst);
                switch (LineNo) {
                    case  1:
                        strcpy(ProgramKey.CompanyName, (const char *) dst);
                        break;

                    case  2:
                        strcpy(ProgramKey.CompanyAddr1, (const char *) dst);
                        break;

                    case  3:
                        strcpy(ProgramKey.CompanyAddr2, (const char *) dst);
                        break;
                    
                    case  4:
                        strcpy(ProgramKey.CompanyCity, (const char *) dst);
                        break;
                        
                    case  5:
                        strcpy(ProgramKey.CompanyState, (const char *) dst);
                        break;

                    case  6:
                        strcpy(ProgramKey.CompanyPostal, (const char *) dst);
                        break;

                    case  7:
                        strcpy(ProgramKey.CompanyCountry, (const char *) dst);
                        break;

                    case  8:
                        strcpy(ProgramKey.CompanyPhone, (const char *) dst);
                        break;

                    case  9:
                        strcpy(ProgramKey.CompanyFax, (const char *) dst);
                        break;

                    case 10:
                        strcpy(ProgramKey.PurchaseDate, (const char *) dst);
                        break;

                    case 11:
                        strcpy(ProgramKey.ExpDate, (const char *) dst);
                        break;

                    case 12:
                        ProgramKey.MaxUsers = atol((const char *) dst);
                        break;

                    case 13:
                        strcpy(ProgramKey.CipherKey, (const char *) dst);
                        break;

                    default:
                        Success = 0;
                        break;
                } 
            } else {
                Success = 0;
            }
        }
        // memset(src, 0, 1024);
        // memset(dst, 0, 1024);
    }
    fclose(fp);
    
    // Check the line count.
    if (LineNo != 13) {
        Success = 0;
    }

    // Check the expiration date of the key.    
/*    printf("Checking the date.\n");
    fflush(stdout);
    if (Success) {
        QString tmpQstr;
        QDate   keyDate;
        tmpQstr.setStr(ProgramKey.ExpDate);

        #ifdef DBDEBUG
        fprintf(stderr, "myDatetoQDate: Source date is %s\n", myDate);
        #endif

	    keyDate.setYMD(
    	  atoi((const char *) tmpQstr.mid(0,4)),
	      atoi((const char *) tmpQstr.mid(5,2)),
	      atoi((const char *) tmpQstr.mid(8,2))
	    );
	    
	    if (keyDate < QDate::currentDate()) {
	        printf("Your program key has expired.\n");
	        exit(-1);
	    }
    }
*/

    // Assign the user Cipher Key
    if (Success) {
        //fprintf(stderr, "CipherKey = '%s'\n", (unsigned char *) ProgramKey.CipherKey);
        set_user_key((unsigned char *) ProgramKey.CipherKey);
        // memcpy(cbc_user, ProgramKey.CipherKey, 8);
        // Make sure the parity is odd...
        // des_set_odd_parity((C_Block *)cbc_user);
    }
    
    if (!Success) {
        fprintf(stderr, "\nError with key file.\n\n");
        exit(-1);
    }
}

