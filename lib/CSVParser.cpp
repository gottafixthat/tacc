/*
** CSVParser.cpp - The CSVParser class
****************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvatel Corporation and R. Marc Lewis.
****************************************************************************/

#include <CSVParser.h>


/**
 * CSVParser()
 *
 * Constructor.
 */
CSVParser::CSVParser()
{
    myDelim = ',';
}

/**
 * ~CSVParser()
 *
 * Destructor.
 */
CSVParser::~CSVParser()
{
}


/**
 * substSep()
 *
 * Given a string, this changes its seperator from one to another, provided
 * that it is not in a block of quotes.
 *
 * Example:  "Fleas, my dog has them",smith,john
 * If splitting on just the commas, then there would be four fields, there
 * are really only three though.  Replacing the ',' with '\n' would allow
 * the split to be done without impacting it.  This function checks for commas
 * in quotes.  Its not overly efficient, though.
 */
QString CSVParser::substSep(const QString &src, const QChar &srch, const QChar &repl)
{
    QString retVal = src.copy();

    bool inBlock = false;
    
    for( uint i = 0; i < retVal.length(); i++ ) {
        if( inBlock ) {
            if( retVal[i] == '\"' ) {
                inBlock = false;   
            }
        } else {
            if( retVal[i] == '\"' ) {
                inBlock = true;
            } else {
                if (retVal[i] == srch) {
                    retVal[i] = repl;
                }
            }
        }   
    }
    return retVal;
}

/**
 * openFile()
 *
 * Given a file name, this attempts to open it for reading.
 */
bool CSVParser::openFile(const QString &fName, bool firstRowIsHeader)
{
    if (myFile.isOpen()) myFile.close();
    myFile.setName(fName);
    myFile.open(IO_ReadOnly);
    myStream = new QTextStream(&myFile);
    if (firstRowIsHeader && myFile.isOpen()) {
        loadRecord();
        myHeader = row();
    }
    return myFile.isOpen();
}

/**
 * setDelimiter()
 *
 * Sets the character delimiter we'll be using on the file.
 */
void CSVParser::setDelimiter(QChar &newDelim)
{
    myDelim = newDelim;
}

/**
 * loadRecord()
 *
 * Loads the next line from the file.  Returns the number of columns or 0 
 * if we have reached the end of the file.
 */
Q_LONG CSVParser::loadRecord()
{
    Q_LONG  retVal = 0;
    if (!myFile.isOpen()) return retVal;
    if (myFile.atEnd()) return retVal;
    
    QString line = myStream->readLine();
    if (line == NULL) return retVal;

    curRow = QStringList::split("\n", substSep(line, myDelim, '\n'), true);
    // Strip the quotes from each of the columns if there are any.
    for (unsigned int i = 0; i < curRow.count(); i++) {
        if (curRow[i][0] == '"' && curRow[i][curRow[i].length()-1] == '"') {
            curRow[i] = curRow[i].mid(1,curRow[i].length()-2);
        }
    }
    retVal = curRow.count();
    return retVal;
}

/**
 * row()
 *
 * Returns the current parsed row.
 */
const QStringList &CSVParser::row()
{
    return curRow;
}

/**
 * header()
 *
 * Returns the header record if it exists.
 */
const QStringList &CSVParser::header()
{
    return myHeader;
}
