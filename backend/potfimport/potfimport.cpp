/* Total Accountability Customer Care (TACC)
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1997-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 *
 * potfimport - PointOne TollFree Import program for importing PointOne's 
 * toll free rates into the AVP database for use with TACC's VoIP Rating
 * system.
 */

#include <stdio.h>
#include <stdlib.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>

#include <CSVParser.h>
#include <ADB.h>
#include <TAATools.h>
#include <Cfg.h>

#include <potfimport.h>

QStringList fileList;

QList<tfrateRecord> tfrateList;
long    carrierID;
int     verbosity;

/**
 * main()
 *
 * Program entry point.
 */
int main(int argc, char **argv)
{
    // Load the TAA Config
    loadTAAConfig();

    // Setup the databases
    ADB::setDefaultHost(cfgVal("TAAMySQLHost"));
    ADB::setDefaultDBase(cfgVal("TAAMySQLDB"));
    ADB::setDefaultUser(cfgVal("TAAMySQLUser"));
    ADB::setDefaultPass(cfgVal("TAAMySQLPass"));

    // Initialize our globals
    carrierID = 0;
    verbosity = 0;

    // Parse our arguments.
    int opt;
    while ((opt = getopt(argc, argv, "cv?i:")) != -1) {
        switch(opt) {
            case 'v':
                verbosity++;
                break;

            case 'i':
                carrierID = atol(optarg);
                break;

            case 'c':   // List Carriers
                listCarriers();
                exit(0);
                break;

            case '?':
            default:    // '?'
                fprintf(stderr, "Usage: %s [-c][-v] -i carrierid file1 [file2 [fileN]]\n", argv[0]);
                exit(-1);
                break;
        }
    }

    // Were we given a filename?
    if (optind >= argc) {
        fprintf(stderr, "No file name specified. Use '%s -?' for usage.\n", argv[0]);
        exit(-1);
    }

    if (!carrierID) {
        fprintf(stderr, "No carrier ID specified. Use '%s -?' for usage.\n", argv[0]);
        exit(-1);
    } else {
        if (!checkCarrierID()) {
            fprintf(stderr, "Invalid carrier ID specified. Use '%s -c' for a list of carriers.\n", argv[0]);
            exit(-1);
        }
    }

    // Get the list of files from the command line
    while (optind < argc) {
        fileList += argv[optind];
        optind++;
    }
    // Verify the files exist, are actually files and that we can read them.
    bool allOkay = true;
    for (int i = 0; i < fileList.size(); i++) {
        QFileInfo   tmpFile(fileList.at(i));
        if (!tmpFile.exists() || !tmpFile.isFile() || !tmpFile.isReadable()) {
            fprintf(stderr, "Unable to open file '%s' for reading\n", fileList.at(i).ascii());
            allOkay = false;
        } else {
            if (!readFile(fileList.at(i))) allOkay = false;
        }
    }
    if (!allOkay) exit(-1);

    fprintf(stderr, "Loaded %d rate records\n", tfrateList.size());

    // Instantiate our application.
    //QCoreApplication a(argc, argv);  

    // Insert the rates
    insertRates();
    
}

/**
 * listCarriers()
 *
 * Lists the carriers from the database.
 */
void listCarriers(void)
{
    ADB     db;
    db.query("select * from VoIPCarriers");
    if (!db.rowCount) {
        fprintf(stderr, "No carriers were found!\n");
        return;
    }
    printf("%-5s %s\n", "ID", "Carrier Name");
    while(db.getrow()) {
        printf("%-5s %s\n", db.curRow["CarrierID"], db.curRow["CarrierName"]);
    }
}

/**
 * checkCarrierID()
 *
 * Checks to make sure the carrier ID is valid.
 */
bool checkCarrierID(void)
{
    bool    retVal = true;
    ADB     db;
    db.query("select * from VoIPCarriers where CarrierID = %ld", carrierID);
    if (!db.rowCount) {
        retVal = false;
    } else if (verbosity) {
        db.getrow();
        printf("Selected Carrier: %s\n", db.curRow["CarrierName"]);
    }

    return retVal;
}
/**
 * readFile()
 *
 * Reads and parses the specified file into memory.
 *
 * Returns true if successful, false if not.
 */
bool readFile(QString filename)
{
    CSVParser   parser;

    if (!parser.openFile(filename, true)) {
        return false;
    }
    if (verbosity) {
        printf("Loading file '%s'...\n", filename.ascii());
    }

    long rowCount = 0;
    // Extract the headers.
    int LATACol     = parser.header().findIndex("LATA");
    int LATANameCol = parser.header().findIndex("LATA_NAME");
    int rateCol     = parser.header().findIndex("RATE");

    while(parser.loadRecord()) {
        rowCount++;
        if (verbosity) {
            if (!(rowCount % 100)) {
                printf("\rLoaded %ld rows...", rowCount);
                fflush(stdout);
            }
        }
        tfrateRecord  rec;
        rec.LATA        = parser.row()[LATACol];
        rec.LATAName    = parser.row()[LATANameCol];
        rec.Rate        = parser.row()[rateCol];
        tfrateList.append(rec);
    }

    if (verbosity) {
        printf("\rLoaded %ld rows from %s\n", rowCount, filename.ascii());
    }
    return true;
}

/**
 * insertRates()
 *
 * Gets the current tforigvers from the carriers table, increments it by one
 * and then adds the rates for this carrier into the database.
 */
void insertRates(void)
{
    ADB         db;
    ADBTable    ratedb("tforigrates");
    QString     timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
    tfrateRecord rec;
    QString     sql;

    db.query("select tforigvers from VoIPCarriers where CarrierID = %ld", carrierID);
    if (!db.rowCount) {
        fprintf(stderr, "Unable to get the table version from the VoIPCarriers table!\n");
        exit(-1);
    }

    db.getrow();
    long tforigVers = (atoi(db.curRow["tforigvers"]));
    tforigVers++;
    if (verbosity) {
        printf("Inserting rate table version %ld\n", tforigVers);
    }

    // Now, walk through the list and insert them all.
    for (int i = 0; i < tfrateList.size(); i++) {
        rec = tfrateList.at(i);
        ratedb.clearData();
        ratedb.setValue("CarrierID",        carrierID);
        ratedb.setValue("tforigvers",       tforigVers);
        ratedb.setValue("lata",             rec.LATA.ascii());
        ratedb.setValue("lataname",         rec.LATAName.ascii());
        ratedb.setValue("rate",             rec.Rate.ascii());
        ratedb.setValue("lastupdated",      timestamp.ascii());
        ratedb.ins();

        if (verbosity) {
            if (!(i % 100)) {
                printf("\rInserted %d records...", i);
                fflush(stdout);
            }
        }
    }
    if (verbosity) {
        printf("\rInserted %d records.\n", tfrateList.size());
    }

    // Update the table version
    db.dbcmd("update VoIPCarriers set tforigvers = %ld where CarrierID = %ld", tforigVers, carrierID);
}


// vim: expandtab
