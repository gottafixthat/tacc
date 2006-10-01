#
# RateCenters - Contains a mapping of the rate centers for DID's.
CREATE TABLE RateCenters (
    RateCenterID    int(11) NOT NULL auto_increment,
    RateCenter      varchar(80) NOT NULL DEFAULT '',
    PRIMARY KEY(RateCenterID)
);

#
# DIDs - Contains all of our DID's that have been assigned to us
# from any of our various sources for them.
CREATE TABLE DIDs (
    DID             int(11) NOT NULL DEFAULT '0',
    NPA             int NOT NULL DEFAULT '0',
    NXX             int NOT NULL DEFAULT '0',
    RateCenterID    int(11) NOT NULL DEFAULT '0',
    Reserved        tinyint NOT NULL DEFAULT '0',
    Assigned        tinyint NOT NULL DEFAULT '0',
    CustomerID      bigint(11) NOT NULL DEFAULT '0',
    LoginID         varchar(64) NOT NULL DEFAULT '0',
    DateAdded       datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
    DateAssigned    datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
    DateReserved    datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
    HoldUntilDate   datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
    PRIMARY KEY(DID),
    INDEX NPANXXIDX (NPA, NXX),
    INDEX RateCenterIDX (RateCenterID)
);

