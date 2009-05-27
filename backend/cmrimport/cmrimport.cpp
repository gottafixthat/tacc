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
 * cmrimport - ChoiceMax Rates Import program for importing PointOne's 
 * choicemax rates into the AVP database for use with TACC's VoIP Rating
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

#include <cmrimport.h>

QStringList fileList;

QList<rateRecord> rateList;
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

    fprintf(stderr, "Loaded %d rate records\n", rateList.size());

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
    int npanxxCol       = parser.header().findIndex("NPANXX");
    int npaCol          = parser.header().findIndex("NPA");
    int nxxCol          = parser.header().findIndex("NXX");
    int locStateCol     = parser.header().findIndex("LOC_STATE");
    int LATAOCNCol      = parser.header().findIndex("LATAOCN");
    int LATACol         = parser.header().findIndex("LATA");
    int LATANameCol     = parser.header().findIndex("LATA_NAME");
    int OCNCol          = parser.header().findIndex("OCN");
    int OCNNameCol      = parser.header().findIndex("OCN_NAME");
    int OCNTypeCol      = parser.header().findIndex("OCN_TYPE");
    int interlataRateCol= parser.header().findIndex("Between_the_States");
    int intralataRateCol= parser.header().findIndex("Within_a_state");

    while(parser.loadRecord()) {
        rowCount++;
        if (verbosity) {
            if (!(rowCount % 100)) {
                printf("\rLoaded %ld rows...", rowCount);
                fflush(stdout);
            }
        }
        rateRecord  rec;
        rec.npanxx          = parser.row()[npanxxCol];
        rec.npa             = parser.row()[npaCol];
        rec.nxx             = parser.row()[nxxCol];
        rec.locState        = parser.row()[locStateCol];
        rec.LATAOCN         = parser.row()[LATAOCNCol];
        rec.LATA            = parser.row()[LATACol];
        rec.LATAName        = parser.row()[LATANameCol];
        rec.OCN             = parser.row()[OCNCol];
        rec.OCNName         = parser.row()[OCNNameCol];
        rec.OCNType         = parser.row()[OCNTypeCol];
        rec.interlataRate   = parser.row()[interlataRateCol];
        rec.intralataRate   = parser.row()[intralataRateCol];
        rateList.append(rec);
    }

    if (verbosity) {
        printf("\rLoaded %ld rows from %s\n", rowCount, filename.ascii());
    }
    return true;
}

/**
 * insertRates()
 *
 * Gets the current termvers from the carriers table, increments it by one
 * and then adds the rates for this carrier into the database.
 */
void insertRates(void)
{
    ADB         db;
    ADBTable    ratedb("VoIPTermRates");
    QString     timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
    rateRecord  rec;
    QString     sql;

    db.query("select termvers from VoIPCarriers where CarrierID = %ld", carrierID);
    if (!db.rowCount) {
        fprintf(stderr, "Unable to get the table version from the VoIPCarriers table!\n");
        exit(-1);
    }

    db.getrow();
    long termVers = (atoi(db.curRow["termvers"]));
    termVers++;
    if (verbosity) {
        printf("Inserting rate table version %ld\n", termVers);
    }

    // Now, walk through the list and insert them all.
    for (int i = 0; i < rateList.size(); i++) {
        rec = rateList.at(i);
        ratedb.clearData();
        ratedb.setValue("CarrierID",        carrierID);
        ratedb.setValue("termvers",         termVers);
        ratedb.setValue("npanxx",           rec.npanxx.ascii());
        ratedb.setValue("npa",              rec.npa.ascii());
        ratedb.setValue("nxx",              rec.nxx.ascii());
        ratedb.setValue("locstate",         rec.locState.ascii());
        ratedb.setValue("lata",             rec.LATA.ascii());
        ratedb.setValue("lataocn",          rec.LATAOCN.ascii());
        ratedb.setValue("lataname",         rec.LATAName.ascii());
        ratedb.setValue("ocn",              rec.OCN.ascii());
        ratedb.setValue("ocnname",          rec.OCNName.ascii());
        ratedb.setValue("ocntype",          rec.OCNType.ascii());
        ratedb.setValue("interlatarate",    rec.interlataRate.ascii());
        ratedb.setValue("intralatarate",    rec.intralataRate.ascii());
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
        printf("\rInserted %d records.\n", rateList.size());
    }

    // Update the table version
    db.dbcmd("update VoIPCarriers set termvers = %ld where CarrierID = %ld", termVers, carrierID);
}


// vim: expandtab
