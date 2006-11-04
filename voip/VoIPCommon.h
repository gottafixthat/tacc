/*
***************************************************************************
**
** VoIPCommon - An interface that allows an admin to maintain DID's
** for our VoIP service.
**
***************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2006, R. Marc Lewis and Avvanta Communications Corp.
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvanta Communications and R. Marc Lewis.
***************************************************************************
*/


#ifndef VoIPCommon_included
#define VoIPCommon_included

// VoIP Service flags, many more will be added to this list.
enum VoIPServiceFlags {
    VSF_VOICEMAIL       = 1,
    VSF_E911            = 2,
    VSF_DIRECTORYASSIST = 3,
    VSF_CALLFORWARDING  = 4,
    VSF_INTERNATIONAL   = 5
};

extern const char *voipServiceFlagText[];

const char *voipServiceFlagStr(int);


#endif // VoIPCommon_included
