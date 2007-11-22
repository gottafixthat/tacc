/* $Id: TAAConfig.h,v 1.1 2003/12/07 01:47:04 marc Exp $
**
** TAAConfig.h  - A _very_ simple interface to config files for programs.
**
**             A config file consists of multiple lines, with keywords
**             on the left, an equals sign, and the values on the right.
**             Whitespace will be stripped from the beginning and end
**             of the lines.
*/

#ifndef TAACONFIG_H
#define TAACONFIG_H

void    cfgInit(void);
void    updateCfgVal(const char *token, const char *val);

#endif // TAACONFIG_H
