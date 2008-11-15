/* $Id: bdes.h,v 1.2 1999/06/24 01:20:36 marc Exp $
**
** Blarg DES routines.  These routines hook into the libdes.a functions, and
** should only be used when creating a program not for export...
*/

#ifndef BDES2_H
#define BDES2_H

#ifdef __cplusplus
extern "C" {
#endif

void set_user_key(unsigned char *UserKey);

void encrypt_string(unsigned char *source, unsigned char *dest, int UseUser);
int  decrypt_string(unsigned char *source, unsigned char *dest, int UseUser);

void encrypt_string2(unsigned char *source, unsigned char *dest);
int  decrypt_string2(unsigned char *source, unsigned char *dest);

void encrypt_string2user(unsigned char *source, unsigned char *dest);
int  decrypt_string2user(unsigned char *source, unsigned char *dest);


/* These are internal routines and should not be called directly. */
void encrypt2(unsigned char *source, unsigned char *dest, unsigned char *Key3);
int  decrypt2(unsigned char *source, unsigned char *dest, unsigned char *Key3);

#ifdef __cplusplus
}
#endif

#endif
