/**
 * CustomerContactsDB.h - Container for managing customer contacts.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef CUSTOMERCONTACTSDB_H
#define CUSTOMERCONTACTSDB_H

#include <TAAStructures.h>
#include <qsqlcursor.h>
#include <qlist.h>


class CustomerContactsDB
{
public:
    CustomerContactsDB();
    ~CustomerContactsDB();

    uint                    loadContact(uint);
    customerContactRecord   getCustomerContact();
    static customerContactList allCustomerContacts(uint);
    void                    clear();
    int                     update();
    int                     insert();
    int                     del();

    void                    setContactID(uint);
    void                    setCustomerID(uint);
    void                    setTag(const QString val)           { custContact.tag = val; };
    void                    setName(const QString val)          { custContact.name = val; };
    void                    setAccess(const QString val)        { custContact.access = val; };
    void                    setInternational(uint val)          { custContact.international = val; };
    void                    setPhoneNumber(const QString val)   { custContact.phoneNumber = val; };
    void                    setEmailAddress(const QString val)  { custContact.emailAddress= val; };
    void                    setActive(uint val)                 { custContact.active = val; };
    void                    setFlags(uint val)                  { custContact.flags = val; };
    void                    setLastModifiedBy(const QString val) { custContact.lastModifiedBy = val; };
    void                    setLastModified(const QDateTime = QDateTime::currentDateTime());

    uint                    contactID()         { return custContact.contactID; };
    uint                    customerID()        { return custContact.customerID; };
    const QString           tag()               { return custContact.tag; };
    const QString           name()              { return custContact.name; };
    const QString           access()            { return custContact.access; };
    uint                    international()     { return custContact.international; };
    const QString           phoneNumber()       { return custContact.phoneNumber; };
    const QString           emailAddress()      { return custContact.emailAddress; };
    uint                    active()            { return custContact.active; };           
    uint                    flags()             { return custContact.flags; };
    const QString           lastModifiedBy()    { return custContact.lastModifiedBy; };
    const QDateTime         lastModified()      { return custContact.lastModified; };

protected:
    customerContactRecord   custContact;
    void                    setCursorValues(QSqlRecord *);
};

#endif

