/* crypto/des/ede_enc.c */
/* Copyright (C) 1995-1996 Eric Young (eay@mincom.oz.au)
 * All rights reserved.
 * 
 * This file is part of an SSL implementation written
 * by Eric Young (eay@mincom.oz.au).
 * The implementation was written so as to conform with Netscapes SSL
 * specification.  This library and applications are
 * FREE FOR COMMERCIAL AND NON-COMMERCIAL USE
 * as long as the following conditions are aheared to.
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.  If this code is used in a product,
 * Eric Young should be given attribution as the author of the parts used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Eric Young (eay@mincom.oz.au)
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

#include "des_locl.h"

void des_ede3_cbc_encrypt(input, output, length, ks1, ks2, ks3, ivec, encrypt)
des_cblock (*input);
des_cblock (*output);
long length;
des_key_schedule ks1;
des_key_schedule ks2;
des_key_schedule ks3;
des_cblock (*ivec);
int encrypt;
	{
	register DES_LONG tin0,tin1;
	register DES_LONG tout0,tout1,xor0,xor1;
	register unsigned char *in,*out;
	register long l=length;
	DES_LONG tin[2];
	unsigned char *iv;

	in=(unsigned char *)input;
	out=(unsigned char *)output;
	iv=(unsigned char *)ivec;

	if (encrypt)
		{
		c2l(iv,tout0);
		c2l(iv,tout1);
		for (; l>0; l-=8)
			{
			if (l >= 8)
				{
				c2l(in,tin0);
				c2l(in,tin1);
				}
			else
				c2ln(in,tin0,tin1,l);
			tin0^=tout0;
			tin1^=tout1;

			IP(tin0,tin1);

			tin[0]=tin0;
			tin[1]=tin1;
			des_encrypt2((DES_LONG *)tin,ks1,DES_ENCRYPT);
			des_encrypt2((DES_LONG *)tin,ks2,DES_DECRYPT);
			des_encrypt2((DES_LONG *)tin,ks3,DES_ENCRYPT);
			tout0=tin[0];
			tout1=tin[1];

			FP(tout1,tout0);

			l2c(tout0,out);
			l2c(tout1,out);
			}
		iv=(unsigned char *)ivec;
		l2c(tout0,iv);
		l2c(tout1,iv);
		}
	else
		{
		c2l(iv,xor0);
		c2l(iv,xor1);
		for (; l>0; l-=8)
			{
			register DES_LONG t0,t1;

			c2l(in,tin0);
			c2l(in,tin1);

			t0=tin0;
			t1=tin1;
			IP(tin0,tin1);  

			tin[0]=tin0;
			tin[1]=tin1;
			des_encrypt2((DES_LONG *)tin,ks3,DES_DECRYPT);
			des_encrypt2((DES_LONG *)tin,ks2,DES_ENCRYPT);
			des_encrypt2((DES_LONG *)tin,ks1,DES_DECRYPT);
			tout0=tin[0];
			tout1=tin[1];

			FP(tout1,tout0); /**/

			tout0^=xor0;
			tout1^=xor1;
			if (l >= 8)
				{
				l2c(tout0,out);
				l2c(tout1,out);
				}
			else
				l2cn(tout0,tout1,out,l);
			xor0=t0;
			xor1=t1;
			}
		iv=(unsigned char *)ivec;
		l2c(xor0,iv);
		l2c(xor1,iv);
		}
	tin0=tin1=tout0=tout1=xor0=xor1=0;
	tin[0]=tin[1]=0;
	}

#ifdef undef /* MACRO */
void des_ede2_cbc_encrypt(input, output, length, ks1, ks2, ivec, enc)
des_cblock (*input);
des_cblock (*output);
long length;
des_key_schedule ks1;
des_key_schedule ks2;
des_cblock (*ivec);
int enc;
	{
	des_ede3_cbc_encrypt(input,output,length,ks1,ks2,ks1,ivec,enc);
	}
#endif
