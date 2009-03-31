/*
** $Id$
**
** mailwrap.cpp - A very small and simple program that runs from cron that
**                scans /var/spool/taamail and mails all of the files in it
**                to the users specified within the file.  It removes the
**                files when it has sent them.
**
**                This should allow TAA running as any user to "become"
**                another user for the purpose of sending mail to users.
**
*****************************************************************************
**
**  (C)opyright 1998, 1999 by R. Marc Lewis and Blarg! Online Services, Inc.
**  All Rights Reserved
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Blarg! Online Services and R. Marc Lewis.
**
*****************************************************************************
*/

#include <mimepp/config.h>
#include <mimepp/protocol.h>
#include <mimepp/string.h>
#include <mimepp/smtp.h>
#include <mimepp/message.h>
#include <mimepp/headers.h>
#include <mimepp/text.h>
#include <mimepp/mboxlist.h>
#include <mimepp/mailbox.h>

#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <qdir.h>
#include <qstring.h>
//Added by qt3to4:
#include <Q3CString>

#include "Cfg.h"
#include <ADB.h>

#define MAXMSGS     1000
#define SLEEPTIME   10
#define INBUFSIZE   1024
#define DEFAULTSMTPHOST    "mail.avvanta.com"
#define BIGBUFSIZE  262144

int msgCount = 0;
char smtpHost[1024];

void scanDir(void);
void scanDb(void);

int main ()
{
    int pid;

    openlog("taamail", LOG_PID|LOG_NDELAY, LOG_INFO);
    for(;;) {
        if ((pid = fork()) == 0) {
            // This is the child process now.
            break;
        } else {
            if (pid >0) {
                wait(NULL); // Wait for the child to exit
            } else {
                syslog(LOG_NOTICE, "Error forking child process");
            }
            sleep(SLEEPTIME);
        }
        sleep(SLEEPTIME);
    }
    strcpy(smtpHost, DEFAULTSMTPHOST);
    // The child picks up here.

    // Load our configuration file first.
    if (!loadCfg("/etc/taa.cf")) {
        if (!loadCfg("/usr/local/etc/taa.cf")) {
            if (!loadCfg("/usr/local/lib/taa.cf")) {
                syslog(LOG_INFO, "Unable to load taa.cf.  Aborting.");
                exit(0);
            }
        }
    }

    if (strlen(cfgVal("SMTPServer"))) {
        strcpy(smtpHost, cfgVal("SMTPServer"));
    }

    scanDir();
    scanDb();
    //syslog(LOG_INFO, "Sent %d messages.", msgCount);
    //closelog();
    exit(0);
}

/*
** scanDir - Scans the directory and gets a listing of all of the files in it.
*/

void scanDir(void)
{
    char     tmpbuf[1024];
    char     fromline[1024];
    char     toaddr[1024];
    Q3CString tmpstr;
    Q3CString tmpstr2;
    FILE     *src;
    QDir     dir("/var/spool/taamail");
    char     *bigbuf;
    int      respCode;
    
    // Our SMTP stuff
    DwSmtpClient    smtp;
    
    const   QFileInfoList   *list = dir.entryInfoList();
    QFileInfoListIterator it(*list);
    QFileInfo *fi;
    QDateTime   curDateTime = QDateTime::currentDateTime();
    // printf(" Bytes, File Name\n");
    while((fi = it.current())) {
        // Make sure its a file and at least SLEEPTIME seconds old
        if (fi->isFile() && (fi->lastModified().secsTo(curDateTime) > SLEEPTIME)) {
            if (!smtp.IsOpen()) {
                if (!smtp.Open(smtpHost)) {
                    syslog(LOG_INFO, "Unable to connect to mail host '%s'", smtpHost);
                    return;
                } else {
                    respCode = smtp.Helo();
                    if (!respCode || respCode >= 500) {
                        smtp.Close();
                        syslog(LOG_INFO, "HELO command failed when talking to mail host '%s'", smtpHost);
                        return;
                    }
                }
            }
            
            // One more check to make sure...
            if (!smtp.IsOpen()) {
                syslog(LOG_INFO, "Our SMTP connection to '%s' was pulled out from under us", smtpHost);
            }
        
            // printf("%10i %s\n", fi->size(), fi->fileName().data());
            sprintf(tmpbuf, "/var/spool/taamail/%s", fi->fileName().data());
            src = fopen(tmpbuf, "r");
            if (src != NULL) {
                // First, lets get the from Line out of the message
                strcpy(fromline, "");
                while(fgets(tmpbuf, sizeof(tmpbuf), src)) {
                    tmpstr.setStr(tmpbuf);
                    if (tmpstr.find("From:", 0, FALSE) == 0) {
                        tmpstr2 = tmpstr.simplifyWhiteSpace();
                        tmpstr  = tmpstr2;
                        tmpstr2 = tmpstr.remove(0, 6);
                        strcpy(fromline, tmpstr2);
                        continue;
                    }
                }
                rewind(src);
                
                // Now, lets get the to Line out of the message
                strcpy(toaddr, "");
                while(fgets(tmpbuf, sizeof(tmpbuf), src)) {
                    tmpstr.setStr(tmpbuf);
                    if (tmpstr.find("To:", 0, FALSE) == 0) {
                        tmpstr2 = tmpstr.simplifyWhiteSpace();
                        tmpstr  = tmpstr2;
                        tmpstr2 = tmpstr.remove(0, 4);
                        // Make sure the "To" address has a domain.
                        if (!tmpstr2.find("@")) {
                            tmpstr2.append("@avvanta.com");
                        }
                        strcpy(toaddr, tmpstr2);
                        continue;
                    }
                }
                rewind(src);
                
                // We need a large buffer to hold the message to scan it.
                bigbuf = (char *) calloc(BIGBUFSIZE, sizeof(char));
                
                // Read the file into our bigbuf
                if (!fread(bigbuf, sizeof(char), BIGBUFSIZE, src)) {
                    syslog(LOG_INFO, "Unable to read from source file '%s'", (const char *) fi->fileName().data());
                    fclose(src);
                    smtp.Close();
                    return;
                }
                // We're done with the source file.
                fclose(src);
                
                if (strlen(fromline)) {
                    sprintf(tmpbuf, "/usr/sbin/sendmail -t -f\"%s\"", fromline);
                } else {
                    sprintf(fromline, "support@avvanta.com");
                    sprintf(tmpbuf, "/usr/sbin/sendmail -t -fsupport@avvanta.com");
                }
                
                if (!strlen(toaddr)) {
                    syslog(LOG_INFO, "File %s has no To: address", (const char *) fi->fileName().data());
                } else {
                    // Set the from address.
                    DwMailbox *fraddr = new DwMailbox(fromline);
                    fraddr->Parse();
                    fraddr->Assemble();
                    DwString fraddrst = fraddr->LocalPart();
                    fraddrst += "@";
                    fraddrst += fraddr->Domain();

                    respCode = smtp.Mail(fraddrst.c_str());
                    if (!respCode || respCode >= 500) {
                        // Try again
                        respCode = smtp.Mail(fraddrst.c_str());
                        if (!respCode || respCode >= 500) {
                    //if (!smtp.Mail(fromline)) {
                            syslog(LOG_INFO, "The SMTP server did not accept the from address of '%s'", fromline);
                            smtp.Close();
                            return;
                        }
                    }

                    // Set the recipient.
                    DwMailbox *rcptaddr = new DwMailbox(toaddr);
                    rcptaddr->Parse();
                    rcptaddr->Assemble();
                    DwString rcptaddrst = rcptaddr->LocalPart();
                    rcptaddrst += "@";
                    if (!strlen(rcptaddr->Domain().c_str())) {
                        rcptaddrst += "avvanta.com";
                    } else {
                        rcptaddrst += rcptaddr->Domain();
                    }

                    respCode = smtp.Rcpt(rcptaddrst.c_str());
                    //respCode = smtp.Rcpt(toaddr);
                    if (!respCode || respCode >= 400) {
                        syslog(LOG_INFO, "The SMTP server did not accept the To address of '%s' in file %s", rcptaddrst.c_str(), (const char *) fi->fileName().data());
                        smtp.Close();
                        return;
                    }

                    // Tell the server we're about to start sending data.
                    respCode = smtp.Data();
                    if (!respCode || respCode >= 500) {
                        syslog(LOG_INFO, "The SMTP server '%s' rejected our DATA command", smtpHost);
                        smtp.Close();
                        return;
                    }

                    // If we've made it here, we're good to send the
                    // message.
                    respCode = smtp.SendData(bigbuf, strlen(bigbuf));
                    if (!respCode || respCode >= 500) {
                        syslog(LOG_INFO, "The SMTP server '%s' had an error with our message body", smtpHost);
                        smtp.Close();
                        return;
                    }

                    // If we've made it here, it is safe to remove the
                    // source file.
                    sprintf(tmpbuf, "/var/spool/taamail/%s", fi->fileName().data());
                    unlink(tmpbuf);
                    
                    syslog(LOG_INFO, "message %s sent from %s to %s\n", (const char *) fi->fileName().data(), fromline, toaddr);
                    ++msgCount;
                }                             
                free((void *)bigbuf);
                
            } else {
                printf("Unable to open /var/spool/taamail/%s\n", fi->fileName().data());
            }
        }
        // sleep(1);
        if (msgCount > MAXMSGS) {
            syslog(LOG_INFO, "Sent %d messages (reached maximum per pass)", MAXMSGS);
            return;
        }
        ++it;
    }
    smtp.Close();
    if (msgCount) syslog(LOG_INFO, "Sent %d messages", msgCount);
}



/*
** scanDb - Scans the database for any email that we may need to send.
*/

void scanDb(void)
{
    char     tmpbuf[1024];
    char     fromline[1024];
    char     toaddr[1024];
    Q3CString tmpstr;
    Q3CString tmpstr2;
    char     *bigbuf;
    int      respCode;
    
    // Set the database defaults
    //ADB::setDebugLevel(9);
    ADB::setDefaultHost(cfgVal("TAAMySQLHost"));
    ADB::setDefaultDBase(cfgVal("TAAMySQLDB"));
    ADB::setDefaultUser(cfgVal("TAAMySQLUser"));
    ADB::setDefaultPass(cfgVal("TAAMySQLPass"));
    
    // Our SMTP stuff
    DwSmtpClient    smtp;

    // Get the email from the database email queue.
    ADB     qdb;
    ADB     ddb;

    qdb.query("select * from EmailQueue");
    if (!qdb.rowCount) return;

    syslog(LOG_INFO, "Found %ld messages to send in the EmailQueue", qdb.rowCount);
    
    // printf(" Bytes, File Name\n");
    while(qdb.getrow()) {
        // Make sure its a file and at least SLEEPTIME seconds old
        // Perhaps later.

        if (!smtp.IsOpen()) {
            if (!smtp.Open(smtpHost)) {
                syslog(LOG_INFO, "Unable to connect to mail host '%s'", smtpHost);
                return;
            } else {
                respCode = smtp.Helo();
                if (!respCode || respCode >= 500) {
                    smtp.Close();
                    syslog(LOG_INFO, "HELO command failed when talking to mail host '%s'", smtpHost);
                    return;
                }
            }
        }
            
        // One more check to make sure...
        if (!smtp.IsOpen()) {
            syslog(LOG_INFO, "Our SMTP connection to '%s' was pulled out from under us", smtpHost);
        }
    
        // printf("%10i %s\n", fi->size(), fi->fileName().data());
        // First, lets get the from Line out of the message
        strcpy(fromline, qdb.curRow["EmailFrom"]);
        
        // Now, lets get the to Line out of the message
        strcpy(toaddr, qdb.curRow["EmailTo"]);
        
        // We need a large buffer to hold the message to scan it.
        //bigbuf = new(char[BIGBUFSIZE]);
        bigbuf = (char *) calloc(BIGBUFSIZE, sizeof(char));
        
        sprintf(bigbuf, "From: %s\nTo: %s\nSubject: %s\n\n%s",
                qdb.curRow["EmailFrom"],
                qdb.curRow["EmailTo"],
                qdb.curRow["EmailSubject"],
                qdb.curRow["TextBody"]);
        
        if (!strlen(toaddr)) {
            syslog(LOG_INFO, "Message %ld has no To: address", atol(qdb.curRow["QueueID"]));
        } else {
            // Set the from address.
            DwMailbox *fraddr = new DwMailbox(fromline);
            fraddr->Parse();
            fraddr->Assemble();
            DwString fraddrst = fraddr->LocalPart();
            fraddrst += "@";
            fraddrst += fraddr->Domain();

            respCode = smtp.Mail(fraddrst.c_str());
            if (!respCode || respCode >= 500) {
                // Try again
                respCode = smtp.Mail(fraddrst.c_str());
                if (!respCode || respCode >= 500) {
            //if (!smtp.Mail(fromline)) {
                    syslog(LOG_INFO, "The SMTP server did not accept the from address of '%s'", fromline);
                    smtp.Close();
                    return;
                }
            }

            // Set the recipient.
            DwMailbox *rcptaddr = new DwMailbox(toaddr);
            rcptaddr->Parse();
            rcptaddr->Assemble();
            DwString rcptaddrst = rcptaddr->LocalPart();
            rcptaddrst += "@";
            if (!strlen(rcptaddr->Domain().c_str())) {
                rcptaddrst += "avvanta.com";
            } else {
                rcptaddrst += rcptaddr->Domain();
            }

            respCode = smtp.Rcpt(rcptaddrst.c_str());
            //respCode = smtp.Rcpt(toaddr);
            if (!respCode || respCode >= 400) {
                syslog(LOG_INFO, "The SMTP server did not accept the To address of '%s'", rcptaddrst.c_str());
                smtp.Close();
                return;
            }

            // Tell the server we're about to start sending data.
            respCode = smtp.Data();
            if (!respCode || respCode >= 500) {
                syslog(LOG_INFO, "The SMTP server '%s' rejected our DATA command", smtpHost);
                smtp.Close();
                return;
            }

            // If we've made it here, we're good to send the
            // message.
            respCode = smtp.SendData(bigbuf, strlen(bigbuf));
            if (!respCode || respCode >= 500) {
                syslog(LOG_INFO, "The SMTP server '%s' had an error with our message body", smtpHost);
                smtp.Close();
                return;
            }

            // If we've made it here, it is safe to remove the
            // source file.
            syslog(LOG_INFO, "Message %s sent from %s to %s\n", qdb.curRow["QueueID"], qdb.curRow["EmailFrom"], qdb.curRow["EmailTo"]);
            ddb.dbcmd("delete from EmailQueue where QueueID = %s", qdb.curRow["QueueID"]);
            ++msgCount;
        }                             
        free((void *)bigbuf);
        
        // sleep(1);
        if (msgCount > MAXMSGS) {
            syslog(LOG_INFO, "Sent %d messages (reached maximum per pass)", MAXMSGS);
            return;
        }
    }
    smtp.Close();
    if (msgCount) syslog(LOG_INFO, "Sent %d messages", msgCount);
}



