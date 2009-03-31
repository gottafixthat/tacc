/*
** CSVParser.h - Class definition for the CSVParser class
****************************************************************************
** Written by R. Marc Lewis, 
**   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
**   All Rights Reserved.
**
**  Unpublished work.  No portion of this file may be reproduced in whole
**  or in part by any means, electronic or otherwise, without the express
**  written consent of Avvatel Corporation and R. Marc Lewis.
****************************************************************************/

#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <Qt3Support/q3textstream.h>
#include <QtCore/QStringList>

class CSVParser
{
public:
    CSVParser();
    ~CSVParser();

    bool    openFile(const QString &fName, bool firstRowIsHeader = false);
    void    setDelimiter(QChar &newDelim);
    Q_LONG  loadRecord();
    const QStringList &row();
    const QStringList &header();

protected:
    QString     substSep(const QString &src, const QChar &srch, const QChar &repl);
    QFile       myFile;
    Q3TextStream *myStream;
    QChar       myDelim;
    QStringList curRow;
    QStringList myHeader;

};

#endif
