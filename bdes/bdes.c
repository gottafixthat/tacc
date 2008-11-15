/* $Id: bdes.c,v 1.1 1999/06/24 01:22:04 marc Exp $
**
** Blarg DES routines
*/


#include <bdes.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "des.h"


// Define our keys to use for the triple DES encryption.
static unsigned char cbc_key1[8] = {0xa4,0x32,0xe1,0x1f,0x70,0x42,0x61,0xbb};
static unsigned char cbc_key2[8] = {0x63,0x19,0x05,0x10,0x99,0x01,0x54,0xfe};
static unsigned char cbc_key3[8] = {0x36,0xfc,0x52,0x84,0x91,0x27,0x23,0x17};
static unsigned char cbc_user[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static unsigned char cbc_iv  [8] = {0xfe,0xcd,0xba,0x98,0x76,0x54,0x32,0x10};


/*
**
** SetUserKey - Sets the cbc_user key to the specified value and sets odd
**              parity.
*/

void set_user_key(unsigned char *UserKey)
{
    memcpy(cbc_user, UserKey, 8);
    /* Make sure the parity is odd */
    des_set_odd_parity((C_Block *)cbc_user);
}


/*
** encrypt_string - Takes the string src and encrypts it.  It stores the
**                  resulting encrypted string in dest.  
**
** NOTE! Dest must be pre-allocated for at least twice the size + 4 of 
**       strlen(src) or problems will occur!  The reason for this is
**       since we're using strings that we want to be printable, we
**       are going to convert dest into hex and return it that way.
**       We also add a simple 4 byte checksum to the end of it so we
**       can verify that the data decrypted okay.
*/

void encrypt_string(unsigned char *source, unsigned char *dest, int UseUser)
{
    des_key_schedule ks1, ks2, ks3;
    des_cblock  iv3;
    unsigned    char tmpstr[64];
    int         err;
    
    // extern unsigned char cbc_key1[8];
    // extern unsigned char cbc_key2[8];
    // extern unsigned char cbc_key3[8];
    // extern unsigned char cbc_user[8];
    // extern unsigned char cbc_iv[8];
    
    unsigned    char * cbc_out;
    unsigned    int  cksum = 0;
    unsigned    char *src;
    int         num = 0;
    unsigned 	int i = 0;

    unsigned char key3[8];

    if (UseUser) {
        memcpy(key3, cbc_user, 8);
    } else {
        memcpy(key3, cbc_key3, 8);
    }



    src = (char *) calloc(strlen((char *) source) * 4, sizeof(char));
    strcpy((char *)src, (const char *) source);

    
    // The source must be evenly divisible by 16...
    // printf("Making the source divisible by 16...\n");
    // fflush(stdout);
/*    while (strlen((const char *)src) % 16) {
        // printf("strlen(src) = %d\n", strlen((const char *)src));
        // fflush(stdout);
        src[strlen((char *)src)+1] = '\0';
        src[strlen((char *)src)]   = ' ';
    }
    // printf("Done making the source divisible by 8...\n");
*/    
    for (i = 0; i < strlen((char *) src); i++) cksum += src[i];
    
    cbc_out = (unsigned char *) calloc(strlen((const char *)src)* 4, sizeof(char));

    // Clear our destination string.
    dest[0] = '\0';

    // Create our key schedules
    err = des_key_sched((C_Block *)cbc_key1, ks1);
    if (err) {
        printf("Key1 error %d\n", err);
        exit(-1);
    }
    
    err = des_key_sched((C_Block *)cbc_key2, ks2);
    if (err) {
        printf("Key2 error %d\n", err);
        exit(-1);
    }
    
    err = des_key_sched((C_Block *)key3, ks3);
    if (err) {
        printf("Key3 error %d\n", err);
        exit(-1);
    }

    // Set up our iv3(?) block
    memcpy(iv3, cbc_iv, sizeof(cbc_iv));
    
    des_ede3_cfb64_encrypt(src, cbc_out, 
      (long )strlen((const char *)src), ks1, ks2, ks3, (C_Block *) iv3, 
      &num, DES_ENCRYPT);

    for (i = 0; i < strlen((const char *)src); i++) {
        sprintf((char *) tmpstr, "%02x", cbc_out[i]);
        // printf("tmpstr = '%s'\n", (char *) tmpstr);
        // fflush(stdout);
        strcat((char *) dest, (const char *) tmpstr);
    }
    
    // Now add the chesksum to the end.
    sprintf((char *) tmpstr, "%04x", cksum);
    strcat((char *) dest, (const char *) tmpstr);


    free(cbc_out);
}

/*
** decrypt_string - Takes the string src and decrypts it.  It stores the
**                  resulting encrypted string in dest.  
**
** NOTE! Dest must be pre-allocated for at least half the size of 
**       strlen(src) or problems will occur!  The reason for this is
**       since we're using strings that we want to be printable, we
**       are going to convert dest into hex and return it that way.
*/

int decrypt_string(unsigned char *source, unsigned char *dest, int UseUser)
{
    des_key_schedule ks1, ks2, ks3;
    des_cblock  iv3;
    char        tmpstr[64];
    int         err;
    unsigned    int cksum = 0;
    unsigned    int cksum2 = 0;
    unsigned    char ckstr[20];
    unsigned    char * cbc_in;
    unsigned    char * src;
    unsigned    char * dst;
    int         num = 0;
    int         RetVal = 0;
    unsigned	int i = 0;
    int j = 0;

    // extern unsigned char cbc_key1[8];
    // extern unsigned char cbc_key2[8];
    // extern unsigned char cbc_key3[8];
    // extern unsigned char cbc_user[8];
    // extern unsigned char cbc_iv[8];
    
    unsigned char key3[8];

    if (UseUser) {
        memcpy(key3, cbc_user, 8);
    } else {
        memcpy(key3, cbc_key3, 8);
    }

    src = (unsigned char *) calloc(strlen((char *) source) *4, sizeof(char));
    dst = (unsigned char *) calloc(strlen((char *) source) *4, sizeof(char));
    strcpy((char *)src, (const char *) source);
    
    memset(ckstr, 0, 16);

    // First things first.  If strlen(src) - 4 / 8 != 0, this is not
    // a valid source...
/*    if ((strlen((const char *) src) - 4) % 16) {
        // Invalid source string...
        return(-1);
    }
*/

    // Get our checksum out of the src.
    j = 0;
    for (i = strlen((const char *) src) - 4; i < strlen((const char *) src); i++) {
        ckstr[j++] = src[i];
    }
    src[strlen((const char *)src)-4] = '\0';
    cksum = strtol((const char *)ckstr, (char **)NULL, 16);
    
    // printf("cksum = 0x%s %d\n", ckstr, cksum);
    // fflush(stdout);


    
    cbc_in = (unsigned char *) calloc(strlen((char *)src)*4, sizeof(char));

    // Clear our destination string.
    dest[0] = '\0';

    // Create our key schedules
    err = des_key_sched((C_Block *)cbc_key1, ks1);
    if (err) {
        printf("Key1 error %d\n", err);
        exit(-1);
    }
    
    err = des_key_sched((C_Block *)cbc_key2, ks2);
    if (err) {
        printf("Key2 error %d\n", err);
        exit(-1);
    }
    
    err = des_key_sched((C_Block *)key3, ks3);
    if (err) {
        printf("Key3 error %d\n", err);
        exit(-1);
    }

    // Set up our iv3(?) block
    memcpy(iv3, cbc_iv, sizeof(cbc_iv));

    // Make our source something usable.
    j = 0;
    for (i = 0; i < strlen((char *)src); i += 2) {
        sprintf(tmpstr, "%c%c", src[i], src[i+1]);
        cbc_in[j++] = strtol(tmpstr, (char **)NULL, 16);
    }
    
    
    des_ede3_cfb64_encrypt(cbc_in, dst, 
      (long )strlen((char *)src)/2, ks1, ks2, ks3, (C_Block *) iv3, 
      &num, DES_DECRYPT);

    // Before we strip the trailing pad from the string, get the checksum
    for (i = 0; i < strlen((char *) dest); i++) cksum2 += dest[i];
    // printf("cksum2 = %d\n", cksum2);

    // Now, strip all of the trailing pad from the string.
/*    while (dst[strlen((const char *)dst)-1] == ' ') {
        dst[strlen((const char *)dst)-1] = '\0';
    }
*/    strcpy((char *)dest, (char *)dst);
    
    free(cbc_in);
    free(src);
    free(dst);
    
    // CkSum checks don't work yet...
    // if (cksum != cksum2) RetVal = -1;
    
    return(RetVal);
}


void encrypt_string2(unsigned char *source, unsigned char *dest)
{
    encrypt2(source, dest, cbc_key3);
}

int decrypt_string2(unsigned char *source, unsigned char *dest)
{
    return decrypt2(source, dest, cbc_key3);
}

void encrypt_string2user(unsigned char *source, unsigned char *dest)
{
    encrypt2(source, dest, cbc_user);
}

int decrypt_string2user(unsigned char *source, unsigned char *dest)
{
    return decrypt2(source, dest, cbc_user);
}


/*
** encrypt2 - Takes the string src and encrypts it.  It stores the
**            resulting encrypted string in dest.  
**
** NOTE! Dest must be pre-allocated for at least twice the size + 4 of 
**       strlen(src) or problems will occur!  The reason for this is
**       since we're using strings that we want to be printable, we
**       are going to convert dest into hex and return it that way.
**       We also add a simple 4 byte checksum to the end of it so we
**       can verify that the data decrypted okay.
*/

void encrypt2(unsigned char *source, unsigned char *dest, unsigned char *Key3)
{
    des_key_schedule ks1, ks2, ks3;
    des_cblock  iv3;
    unsigned    char tmpstr[64];
    int         err;
    
    // extern unsigned char cbc_key1[8];
    // extern unsigned char cbc_key2[8];
    // extern unsigned char cbc_key3[8];
    // extern unsigned char cbc_user[8];
    // extern unsigned char cbc_iv[8];
    
    unsigned    char * cbc_out;
    unsigned    int  cksum = 0;
    unsigned    char *src;
    int         num = 0;
    unsigned int i = 0;
    time_t		timeStamp;
    struct tm	*curTime;

    /*
    unsigned char key3[8];
    memcpy(key3, cbc_key3, 8);
    */


    src = (unsigned char *) calloc(strlen((char *) source) *4, sizeof(char));
    strcpy((char *)src, (const char *) source);

	tzset();
	timeStamp = time(NULL);
    curTime = localtime(&timeStamp);

    /* Okay, now we're going to prepend a timestamp onto our source string 
    ** Which will be a total of 14 characters long (for removal later).
    */
    sprintf(src, "%02d%02d%02d%02d%02d%04d%s", 
      curTime->tm_sec, 
      curTime->tm_min, 
      curTime->tm_hour,
      curTime->tm_mday,
      curTime->tm_mon,
      curTime->tm_year,
      (const char *) source);

    for (i = 0; i < strlen((char *) src); i++) cksum += src[i];
    
    cbc_out = (unsigned char *) calloc(strlen((const char *)src)*4, sizeof(char));

    // Clear our destination string.
    dest[0] = '\0';

    // Create our key schedules
    err = des_key_sched((C_Block *)cbc_key1, ks1);
    if (err) {
        printf("Key1 error %d\n", err);
        exit(-1);
    }
    
    err = des_key_sched((C_Block *)cbc_key2, ks2);
    if (err) {
        printf("Key2 error %d\n", err);
        exit(-1);
    }
    
    err = des_key_sched((C_Block *)Key3, ks3);
    if (err) {
        printf("Key3 error %d\n", err);
        exit(-1);
    }

    // Set up our iv3(?) block
    memcpy(iv3, cbc_iv, sizeof(cbc_iv));
    
    des_ede3_cfb64_encrypt(src, cbc_out, 
      (long )strlen((const char *)src), ks1, ks2, ks3, (C_Block *) iv3, 
      &num, DES_ENCRYPT);

    for (i = 0; i < strlen((const char *)src); i++) {
        sprintf((char *) tmpstr, "%02x", cbc_out[i]);
        // printf("tmpstr = '%s'\n", (char *) tmpstr);
        // fflush(stdout);
        strcat((char *) dest, (const char *) tmpstr);
    }
    
    //fprintf(stderr, "bdes.c encrypt: Source = '%s'\n", (char *) source);
    //fprintf(stderr, "bdes.c encrypt: Target = '%s'\n", (char *) dest);


    // Now add the chesksum to the end.
    sprintf((char *) tmpstr, "%04x", cksum);
    strcat((char *) dest, (const char *) tmpstr);

    free(cbc_out);
}

/*
** decrypt2 - Takes the string src and decrypts it.  It stores the
**            resulting encrypted string in dest.  
**
** NOTE! Dest must be pre-allocated for at least half the size of 
**       strlen(src) or problems will occur!  The reason for this is
**       since we're using strings that we want to be printable, we
**       are going to convert dest into hex and return it that way.
*/

int decrypt2(unsigned char *source, unsigned char *dest, unsigned char *Key3)
{
    des_key_schedule ks1, ks2, ks3;
    des_cblock  iv3;
    char        tmpstr[64];
    int         err;
    unsigned    int cksum = 0;
    unsigned    int cksum2 = 0;
    unsigned    char ckstr[20];
    unsigned    char * cbc_in;
    unsigned    char * src;
    unsigned    char * dst;
    unsigned    char * dst2;
    int         num = 0;
    int         RetVal = 0;
    int j = 0;
    unsigned	int i = 0;

    /*
    unsigned char key3[8];
    memcpy(key3, cbc_key3, 8);
    */

    src = (unsigned char *) calloc(strlen((char *) source) *4, sizeof(char));
    dst = (unsigned char *) calloc(strlen((char *) source) *4, sizeof(char));
    strcpy((char *)src, (const char *) source);
    
    memset(ckstr, 0, 16);

    // First things first.  If strlen(src) - 4 / 8 != 0, this is not
    // a valid source...
/*    if ((strlen((const char *) src) - 4) % 16) {
        // Invalid source string...
        return(-1);
    }
*/

    // Get our checksum out of the src.
    j = 0;
    for (i = strlen((const char *) src) - 4; i < strlen((const char *) src); i++) {
        ckstr[j++] = src[i];
    }
    src[strlen((const char *)src)-4] = '\0';
    cksum = strtol((const char *)ckstr, (char **)NULL, 16);
    
    // printf("cksum = 0x%s %d\n", ckstr, cksum);
    // fflush(stdout);


    
    cbc_in = (unsigned char *) calloc(strlen((char *)src)*4, sizeof(char));

    // Clear our destination string.
    dest[0] = '\0';

    // Create our key schedules
    err = des_key_sched((C_Block *)cbc_key1, ks1);
    if (err) {
        printf("Key1 error %d\n", err);
        exit(-1);
    }
    
    err = des_key_sched((C_Block *)cbc_key2, ks2);
    if (err) {
        printf("Key2 error %d\n", err);
        exit(-1);
    }
    
    err = des_key_sched((C_Block *)Key3, ks3);
    if (err) {
        printf("Key3 error %d\n", err);
        exit(-1);
    }

    // Set up our iv3(?) block
    memcpy(iv3, cbc_iv, sizeof(cbc_iv));

    // Make our source something usable.
    j = 0;
    for (i = 0; i < strlen((char *)src); i += 2) {
        sprintf(tmpstr, "%c%c", src[i], src[i+1]);
        cbc_in[j++] = strtol(tmpstr, (char **)NULL, 16);
    }
    
    
    des_ede3_cfb64_encrypt(cbc_in, dst, 
      (long )strlen((char *)src)/2, ks1, ks2, ks3, (C_Block *) iv3, 
      &num, DES_DECRYPT);

    // Before we strip the trailing pad from the string, get the checksum
    for (i = 0; i < strlen((char *) dest); i++) cksum2 += dest[i];
    // printf("cksum2 = %d\n", cksum2);

    // Now, strip all of the trailing pad from the string.
/*    while (dst[strlen((const char *)dst)-1] == ' ') {
        dst[strlen((const char *)dst)-1] = '\0';
    }
*/  
    dst2 = dst;  
    dst2 += 14;
    strcpy((char *)dest, (char *)dst2);

    //fprintf(stderr, "bdes.c decrypt: Source = '%s'\n", (char *) source);
    //fprintf(stderr, "bdes.c decrypt: Target = '%s'\n", (char *) dest);
    //fprintf(stderr, "bdes.c decrypt: Working= '%s'\n", (char *) dst);
    
    free(cbc_in);
    free(src);
    free(dst);
    
    // CkSum checks don't work yet...
    // if (cksum != cksum2) RetVal = -1;
    
    return(RetVal);
}

