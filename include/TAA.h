/*
** $Id: TAA.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
***************************************************************************
**
** TAA.h - This header defines several static defines.
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
** $Log: TAA.h,v $
** Revision 1.1  2003/12/07 01:47:04  marc
** New CVS tree, all cleaned up.
**
**
*/

#ifndef TAA_H
#define TAA_H

//#define TEMPLATE_PATH "/usr/local/lib/taa"


typedef struct voicemail_disposition {
    int     dispid;
    char    code[32];
    char    description[80];
};

#define MAX_VOICEMAIL_CODE 7
static struct voicemail_disposition voicemail_codes[] = {
    {0, "New", "New/Pending"},
    {1, "RCSWC", "Returned Call, Spoke with Person"},
    {2, "RCLVM", "Returned Call, Left Voice Mail"},
    {3, "RCLB", "Returned Call, Line Busy"},
    {4, "RCNA", "Returned Call, No Answer"},
    {5, "RVE", "Responded via Email"},
    {6, "NICG", "No Contact Information Given"},
    {7, "RCBCI", "Returned Call, Bad Contact Information"},
    {-1, "", ""}
};

#endif

