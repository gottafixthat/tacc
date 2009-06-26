/**************************************************************************
**
** cdrs.cpp - Main program setup and entry point.
**
***************************************************************************
** Written by R. Marc Lewis <marc@avvatel.com>
**   (C)opyright 2008, R. Marc Lewis
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of R. Marc Lewis or Avvatel Corporation.
***************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <getopt.h>

#include <QtCore/QCoreApplication>
#include <QtNetwork/QtNetwork>
#include <QtCore/QTextStream>

#include <TAATools.h>
#include <Cfg.h>

#include "cdrs.h"
#include "ratecdrs.h"

// Function prototypes

int main(int, char **);

cdrsApp::cdrsApp(int &argc, char **argv) : QCoreApplication(argc, argv)
{
    //QTimer  *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(timerTest()));
    //timer->start(5000);
}

/* showHelp - Shows a list of command line options to the user.
 */
void showHelp(void)
{
    QTextStream out(stdout);
    out << "Usage:\n";
    out << "\t-h Display this help\n";
    out << "\t-r Rate CDRs that have not yet been rated\n";
    out << "\n";
}

/*
** main - Program entry point.
*/

int main(int argc, char **argv)
{
    uid_t   myUID;
    passwd  *pent;
	int     opt;

    loadTAAConfig(true, false);

    // Initialize our Qt Application
    cdrsApp a(argc, argv);
    a.setApplicationName("cdrs");

    // Get our home directory so we can check for a config
    // file there for development purposes.
    myUID = getuid();
    pent = getpwuid(myUID);
    if (pent == NULL) {
        fprintf(stderr, "\nUnable to get user information.  Aborting.\n");
        exit(-1);
    }

    // Get our options
    if (argc <= 1) {
        showHelp();
        exit(0);
    }


    // Walk through the options and do anything we need to do...
    while((opt = getopt(argc, argv, "hrqmv")) != EOF) {
        switch(opt) {
            case    'h':
                showHelp();
                exit(0);
                break;

            case    'r':
                rateCDRs();
                break;

            case    'q':
                //QuietMode = 1;
                break;

            case    'v':
                //beVerbose = 1;
                break;

            default:
                //fprintf(stderr, "Invalid argument.\n");
                exit(-1);
                break;
        }
    }

    //a.exec();

    printf("Hello world.\n");
}

