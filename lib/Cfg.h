/*
** $Id: Cfg.h,v 1.1.1.1 2001/03/01 01:19:04 marc Exp $
**
***************************************************************************
**
** Cfg.h - Definitions for the simple Config file library.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2000, R. Marc Lewis and Blarg! Oline Services, Inc.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
***************************************************************************
** $Log: Cfg.h,v $
** Revision 1.1.1.1  2001/03/01 01:19:04  marc
** First checked in version of the BTools library
**
**
*/

#ifndef CFG_H
#define CFG_H

#include <ext/hash_map>
#include <string>

void       setCfgDelimiter(const char *newdelim);
int        loadCfg(const char *cfgFile);
const char *cfgVal(const char *Token);
void       setCfgVal(const char *Token, const char *dataVal);

#endif // CFG_H

