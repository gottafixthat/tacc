#include <loadKey.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QtCore/QDateTime>
#include <bdes.h>

#define TACT_KEYFILE "/usr/local/lib/tact.key"

const char *gKey[] = {"7756cb8af52d2479cfa70bb201ede82ef7600c7b14da128053eb148e0986",
                "6514e5d6b24e6b4ebe0d3ca3faf165c67ef41f1cc74268e9f5211fb5b9ecd631059b",
                "151a8ad8b22c2416d1066aad46fc02e7e3e0131f2f4bfa1c6ad7624089ddb1800400",
                "775fc694f77a7153fbcd3058e8db1424eee946fd716447dfcde78201c72c18b80634",
                "627b8ad8b22c2416ddede18ffc3b14c65666cb1541e1d04dae7da3b5bde15f210458",
                "0c029ac8aa213505a7f403ae498fa7219eb345961afa514727c5b030404234bb04c7",
                "60698ad8b22c24163afd799cb64797050f7f9e474861efc3c759c149b7574c640468",
                "01089fd7a63c351b1dfa9d0749f6370927cace614c07b892178c569825213ba804e0",
                "01089fd7a63c351b1df59b0749f6370921d73244e001a4b8feed0281e7f6a3dd04e1",
                "040393c0bf3c331b0fca0ca848e0ca9461d336d9046306b4526a63033308259204c0",
                "050a9ac8bf3c341bb4e6558e9beb07002d05b2e41d20bc20437c4aa9ecf5f423049a",
                "040a9ac8a22c2416ec45afcf3077ee523dc34f39d6c9e601a05c78a476ac2cc40451",
                "070b99cca138655d7a448e130de28f79451882279c281531407ae4af2f009e3204fd",
                ""};

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
    //FILE        *fp;
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
    
    /*
    fp = fopen(TACT_KEYFILE, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to load key file '%s'.  Exiting.\n", TACT_KEYFILE);
        exit(-1);
    }
    */

    while (strlen(gKey[LineNo])) {
        strcpy((char *) src, gKey[LineNo]);
        //while (fgets((char *)src, 1000, fp) != NULL) {
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
    //fclose(fp);
    
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

