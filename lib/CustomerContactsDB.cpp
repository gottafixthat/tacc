/**
 * CustomerContactsDB.cpp - Class file for manipulating the data in the
 * CustomerContacts table.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#include <CustomerContactsDB.h>
#include <TAATools.h>
#include <QSqlDbPool.h>
#include <qsqlcursor.h>

/**
 * CustomerContactsDB()
 *
 * Constructor.
 */
CustomerContactsDB::CustomerContactsDB()
{
}

/**
 * ~CustomerContactsDB()
 *
 * Destructor.
 */
CustomerContactsDB::~CustomerContactsDB()
{
}

/**
 * loadContact()
 *
 * Loads the requested CustomerContact record by ID.
 *
 * Returns the ContactID if successful, 0 if not.
 */
uint CustomerContactsDB::loadContact(uint id)
{
    uint            retVal = 0;
    QSqlDbPool      dbpool;
    QSqlCursor      contacts("CustomerContacts", true, dbpool.qsqldb());
    QSqlIndex       prikey = contacts.primaryIndex();

    contacts.setValue("ContactID",  id);
    if (contacts.select(prikey, prikey)) {
        if (contacts.next()) {
            custContact.contactID       = contacts.value("ContactID").toUInt();
            custContact.customerID      = contacts.value("CustomerID").toUInt();
            custContact.tag             = contacts.value("Tag").toString();
            custContact.name            = contacts.value("Name").toString();
            custContact.access          = contacts.value("Access").toString();
            custContact.international   = contacts.value("International").toInt();
            custContact.phoneNumber     = contacts.value("PhoneNumber").toString();
            custContact.emailAddress    = contacts.value("EmailAddress").toString();
            custContact.active          = contacts.value("Active").toInt();
            custContact.flags           = contacts.value("Flags").toInt();
            custContact.sendStatements  = contacts.value("SendStatements").toInt();
            custContact.lastModifiedBy  = contacts.value("LastModifiedBy").toString();
            custContact.lastModified    = QDateTime::fromString(contacts.value("LastModifiedBy").toString(), Qt::ISODate);
            retVal = id;

            // Go throug the flags and create our flagList
            custContact.flagList.clear();
            if (!custContact.flags) custContact.flagList += "None";
            if (custContact.flags & (customerContactFlags) Billing) custContact.flagList += "Billing";
        }
    }

    return retVal;
}

/**
 * allCustomerContacts()
 *
 * Returns a QList of all of the customer contacts found in the database
 * for a specific customer ID.
 */
customerContactList CustomerContactsDB::allCustomerContacts(uint id)
{
    QSqlDbPool                      dbpool;
    QSqlCursor                      contacts("CustomerContacts", true, dbpool.qsqldb());
    QSqlIndex                       prikey = contacts.primaryIndex();
    QList<customerContactRecord>    retVal;
    QString                         selStr;

    contacts.setValue("ContactID",  id);
    selStr = "CustomerID=";
    selStr += QString::number(id);
    if (contacts.select(selStr)) {
        while (contacts.next()) {
            customerContactRecord   *contactRec = new customerContactRecord;
            contactRec->contactID       = contacts.value("ContactID").toUInt();
            contactRec->customerID      = contacts.value("CustomerID").toUInt();
            contactRec->tag             = contacts.value("Tag").toString();
            contactRec->name            = contacts.value("Name").toString();
            contactRec->access          = contacts.value("Access").toString();
            contactRec->international   = contacts.value("International").toInt();
            contactRec->phoneNumber     = contacts.value("PhoneNumber").toString();
            contactRec->emailAddress    = contacts.value("EmailAddress").toString();
            contactRec->active          = contacts.value("Active").toInt();
            contactRec->flags           = contacts.value("Flags").toInt();
            contactRec->sendStatements  = contacts.value("SendStatements").toInt();
            contactRec->lastModifiedBy  = contacts.value("LastModifiedBy").toString();
            contactRec->lastModified    = QDateTime::fromString(contacts.value("LastModifiedBy").toString(), Qt::ISODate);

            // Go throug the flags and create our flagList
            contactRec->flagList.clear();
            if (!contactRec->flags) contactRec->flagList += "None";
            if (contactRec->flags & (customerContactFlags) Billing) contactRec->flagList += "Billing";

            retVal.append(contactRec);
        }
    }

    return retVal;
}

/**
 * getCustomerContact()
 *
 * Returns the currently loaded contact record.
 */
customerContactRecord CustomerContactsDB::getCustomerContact()
{
    return custContact;
}

/**
 * clear()
 *
 * Clears all of the data in custContact.
 */
void CustomerContactsDB::clear()
{
    custContact.contactID       = 0;
    custContact.customerID      = 0;
    custContact.tag             = "";
    custContact.name            = "";
    custContact.access          = "";
    custContact.international   = 0;
    custContact.phoneNumber     = "";
    custContact.emailAddress    = "";
    custContact.active          = 0;
    custContact.flags           = 0;
    custContact.sendStatements  = 0;
    custContact.lastModifiedBy  = "";
    custContact.lastModified    = QDateTime::currentDateTime();
}

/**
 * setCursorValues()
 *
 * Given a pointer to a QSqlCursor object, this will assign
 * all of the values but contactID to it.
 */
void CustomerContactsDB::setCursorValues(QSqlRecord *rec)
{
    rec->setValue("CustomerID",         custContact.customerID);
    rec->setValue("Tag",                custContact.tag);
    rec->setValue("Name",               custContact.name);
    rec->setValue("Access",             custContact.access);
    rec->setValue("International",      custContact.international);
    rec->setValue("PhoneNumber",        custContact.phoneNumber);
    rec->setValue("EmailAddress",       custContact.emailAddress);
    rec->setValue("Active",             custContact.active);
    rec->setValue("Flags",              custContact.flags);
    rec->setValue("SendStatements",     custContact.sendStatements);
    rec->setValue("LastModifiedBy",     custContact.lastModifiedBy);
    rec->setValue("LastModified",       custContact.lastModified);
}

/**
 * update()
 *
 * This will write a modified customerContact record back to the database.
 */
int CustomerContactsDB::update()
{
    int retVal = 0;

    
    QSqlDbPool      dbpool;
    QSqlCursor      contacts("CustomerContacts", true, dbpool.qsqldb());
    QSqlIndex       prikey = contacts.primaryIndex();

    contacts.setValue("ContactID",  custContact.contactID);
    if (contacts.select(prikey, prikey)) {
        if (contacts.next()) {
            QSqlRecord  *buf = contacts.primeUpdate();
            setCursorValues(buf);
            retVal = contacts.update();
        }
    }

    return retVal;
}

/**
 * insert()
 *
 * This will insert a new customerContact record into the database.
 */
int CustomerContactsDB::insert()
{
    int retVal = 0;

    
    QSqlDbPool      dbpool;
    QSqlCursor      contacts("CustomerContacts", true, dbpool.qsqldb());

    QSqlRecord *buf = contacts.primeInsert();
    setCursorValues(buf);
    retVal = contacts.insert();

    return retVal;
}

/**
 * del()
 *
 * Delete the currently loaded customerContact record.
 */
int CustomerContactsDB::del()
{
    QSqlDbPool  dbpool;
    QSqlQuery   q(dbpool.qsqldb());
    q.prepare("delete from CustomerContacts where ContactID = :contactID");
    q.bindValue(":contactID", custContact.contactID);
    q.exec();

}

/**
 * setContactID()
 */
void CustomerContactsDB::setContactID(uint val)
{
    custContact.contactID = val;
}

/**
 * setCustomerID()
 */
void CustomerContactsDB::setCustomerID(uint val)
{
    custContact.customerID = val;
}

/**
 * setLastModified()
 */
void CustomerContactsDB::setLastModified(const QDateTime val)
{
    custContact.lastModified = val;
}

