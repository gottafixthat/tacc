/**
 * seanetimport.cpp
 *
 * One time program that will take the seanet data and migrate it
 * to tacc.
 */

#include <CSVParser.cpp>

main()
{
    CSVParser   parser;

    parser.openFile("SeanetBillables.csv", true);
    while(parser.loadRecord()) {
        char colTitle[1024];
        QString tmpStr;
        //fprintf(stderr, "Loaded a record with %d columns...\n", parser.row().count());
        for (unsigned int i = 0; i < parser.row().count(); i++) {
            if (i <= parser.header().count()) {
                QStringList tmpHeaders = parser.header();
                QString tmpHeader = tmpHeaders[i];
                strcpy(colTitle, tmpHeader.ascii());
            } else {
                sprintf(colTitle, "Column %d", i);
            }
            printf("%20s: %s\n", colTitle, parser.row()[i].ascii());
        }
        printf("\n");
    }
}


