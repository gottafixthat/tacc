####### This section is automatically generated

USEDES	= 1

INCDIR	=  $(QTDIR)
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
CFLAGS += -I../customers
CFLAGS += -I../lib
CFLAGS += -I../taccCore -I../taccGui
CFLAGS += -DADBQT
LFLAGS  = -lqt -lmysqlclient -lcrypt -L$(QTDIR)/lib -shared
LFLAGS += -L../lib -lblarg
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTMOC)

# CFLAGS += -DDBDEBUG
CFLAGS += -DQT3_SUPPORT

ifdef USEDES
	CFLAGS	+= -DUSEDES -I../bdes
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	Report.cpp\
		ARReport.cpp\
		LoginTypeReport.cpp\
		LoginCountReport.cpp\
		CallLogReport.cpp\
		LoginsToWipeReport.cpp\
		LoginAvgReport.cpp\
		RatePlanReport.cpp\
		RatePlanDetailReport.cpp\
		ModemUsageReport.cpp\
		BandwidthUsageReport.cpp\
		ModemTotalsReport.cpp\
		BandwidthSummaryReport.cpp\
		CityReport.cpp\
		CityDetailReport.cpp\
		CityFilters.cpp\
		UnreleasedDomainsReport.cpp\
		AllDomainsReport.cpp\
		DNS_ManagedReport.cpp\
		DNS_UnmanagedReport.cpp\
		ActiveDomainsReport.cpp\
		ActiveDomainsDetail.cpp\
		AsteriskCDRReport.cpp\
		SalesReports.cpp\
		ccPaymentsByTypeReport.cpp\
		ccPaymentDetailReport.cpp\
		CheckPaymentsReport.cpp\
		GeneralLedgerReport.cpp\
		AgingReport.cpp\
		AllReports.cpp

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_Report.cpp\
		moc_ARReport.cpp\
		moc_LoginTypeReport.cpp\
		moc_LoginCountReport.cpp\
		moc_CallLogReport.cpp\
		moc_LoginsToWipeReport.cpp\
		moc_LoginAvgReport.cpp\
		moc_RatePlanReport.cpp\
		moc_RatePlanDetailReport.cpp\
		moc_ModemUsageReport.cpp\
		moc_BandwidthUsageReport.cpp\
		moc_ModemTotalsReport.cpp\
		moc_BandwidthSummaryReport.cpp\
		moc_CityReport.cpp\
		moc_CityDetailReport.cpp\
		moc_CityFilters.cpp\
		moc_UnreleasedDomainsReport.cpp\
		moc_AllDomainsReport.cpp\
		moc_DNS_ManagedReport.cpp\
		moc_DNS_UnmanagedReport.cpp\
		moc_ActiveDomainsReport.cpp\
		moc_ActiveDomainsDetail.cpp\
		moc_AsteriskCDRReport.cpp\
		moc_SalesReports.cpp\
		moc_ccPaymentsByTypeReport.cpp\
		moc_ccPaymentDetailReport.cpp\
		moc_CheckPaymentsReport.cpp\
		moc_GeneralLedgerReport.cpp\
		moc_AgingReport.cpp\
		moc_AllReports.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

# TARGET	=	qtacct

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	echo Compiling  $<
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) report-lib

report-lib: $(OBJECTS)
	ar r libreports.a $(OBJECTS) $(OBJMETA)
	ranlib libreports.a

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

#depend:
#	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% *.a #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_Report.cpp: Report.h
	$(MOC) Report.h -o moc_Report.cpp

moc_ARReport.cpp: ARReport.h
	$(MOC) ARReport.h -o moc_ARReport.cpp

moc_LoginTypeReport.cpp: LoginTypeReport.h
	$(MOC) LoginTypeReport.h -o moc_LoginTypeReport.cpp

moc_LoginCountReport.cpp: LoginCountReport.h
	$(MOC) LoginCountReport.h -o moc_LoginCountReport.cpp

moc_CallLogReport.cpp: CallLogReport.h
	$(MOC) CallLogReport.h -o moc_CallLogReport.cpp

moc_LoginsToWipeReport.cpp: LoginsToWipeReport.h
	$(MOC) LoginsToWipeReport.h -o moc_LoginsToWipeReport.cpp

moc_LoginAvgReport.cpp: LoginAvgReport.h
	$(MOC) LoginAvgReport.h -o moc_LoginAvgReport.cpp

moc_RatePlanReport.cpp: RatePlanReport.h
	$(MOC) RatePlanReport.h -o moc_RatePlanReport.cpp

moc_RatePlanDetailReport.cpp: RatePlanDetailReport.h
	$(MOC) RatePlanDetailReport.h -o moc_RatePlanDetailReport.cpp

moc_ModemUsageReport.cpp: ModemUsageReport.h
	$(MOC) ModemUsageReport.h -o moc_ModemUsageReport.cpp

moc_BandwidthUsageReport.cpp: BandwidthUsageReport.h
	$(MOC) BandwidthUsageReport.h -o moc_BandwidthUsageReport.cpp

moc_ModemTotalsReport.cpp: ModemTotalsReport.h
	$(MOC) ModemTotalsReport.h -o moc_ModemTotalsReport.cpp

moc_BandwidthSummaryReport.cpp: BandwidthSummaryReport.h
	$(MOC) BandwidthSummaryReport.h -o moc_BandwidthSummaryReport.cpp

moc_CityReport.cpp: CityReport.h
	$(MOC) CityReport.h -o moc_CityReport.cpp

moc_CityDetailReport.cpp: CityDetailReport.h
	$(MOC) CityDetailReport.h -o moc_CityDetailReport.cpp

moc_CityFilters.cpp: CityFilters.h
	$(MOC) CityFilters.h -o moc_CityFilters.cpp

moc_UnreleasedDomainsReport.cpp: UnreleasedDomainsReport.h
	$(MOC) UnreleasedDomainsReport.h -o moc_UnreleasedDomainsReport.cpp

moc_AllDomainsReport.cpp: AllDomainsReport.h
	$(MOC) AllDomainsReport.h -o moc_AllDomainsReport.cpp

moc_DNS_ManagedReport.cpp: DNS_ManagedReport.h
	$(MOC) DNS_ManagedReport.h -o moc_DNS_ManagedReport.cpp

moc_DNS_UnmanagedReport.cpp: DNS_UnmanagedReport.h
	$(MOC) DNS_UnmanagedReport.h -o moc_DNS_UnmanagedReport.cpp

moc_ActiveDomainsReport.cpp: ActiveDomainsReport.h
	$(MOC) ActiveDomainsReport.h -o moc_ActiveDomainsReport.cpp

moc_ActiveDomainsDetail.cpp: ActiveDomainsDetail.h
	$(MOC) ActiveDomainsDetail.h -o moc_ActiveDomainsDetail.cpp

moc_AsteriskCDRReport.cpp: AsteriskCDRReport.h
	$(MOC) AsteriskCDRReport.h -o moc_AsteriskCDRReport.cpp

moc_ccPaymentsByTypeReport.cpp: ccPaymentsByTypeReport.h
	$(MOC) ccPaymentsByTypeReport.h -o moc_ccPaymentsByTypeReport.cpp

moc_ccPaymentDetailReport.cpp: ccPaymentDetailReport.h
	$(MOC) ccPaymentDetailReport.h -o moc_ccPaymentDetailReport.cpp

moc_CheckPaymentsReport.cpp: CheckPaymentsReport.h
	$(MOC) CheckPaymentsReport.h -o moc_CheckPaymentsReport.cpp

moc_GeneralLedgerReport.cpp: GeneralLedgerReport.h
	$(MOC) GeneralLedgerReport.h -o moc_GeneralLedgerReport.cpp

moc_AgingReport.cpp: AgingReport.h
	$(MOC) AgingReport.h -o moc_AgingReport.cpp

moc_SalesReports.cpp: SalesReports.h
	$(MOC) SalesReports.h -o moc_SalesReports.cpp

moc_AllReports.cpp: AllReports.h
	$(MOC) AllReports.h -o moc_AllReports.cpp

# DO NOT DELETE THIS LINE -- make depend depends on it.
