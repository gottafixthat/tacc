####### This section is automatically generated

USEDES  = 1

INCDIR	=  /usr/lib/qt/include
# CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.
CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I./include/
LFLAGS  = -lqt -lblargdb -L./lib -lBTools -lBCGI
LFLAGS  += -L$(MYSQLLIBPATH) -lmysqlclient
SHELL   = /bin/sh
CC	= gcc
MOC	= /usr/bin/moc

####### End of automatically generated section

####### Files

# SUBDIRS	= lib
ifdef USEDES
	# SUBDIRS	+= libdes bdes
	export USEDES=1
endif

# SUBDIRS += order2ps
#SUBDIRS += taamaild qwestconnect openissues
SUBDIRS += qwestconnect
#SUBDIRS += openissues

SOURCES =	

HEADERS =	

OBJECTS =	

SRCMETA =	

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SUBDIRS)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

depend:
	set -e; for i in $(SUBDIRS); do cd $$i ; $(MAKE) depend ; cd ..; done
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	set -e; for i in $(SUBDIRS); do cd $$i ; $(MAKE) clean ; cd ..; done
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_qtacct.cpp: qtacct.h
	$(MOC) qtacct.h -o moc_qtacct.cpp

moc_global.cpp: global.h
	$(MOC) global.h -o moc_global.cpp

moc_VendorTypes.cpp: VendorTypes.h
	$(MOC) VendorTypes.h -o moc_VendorTypes.cpp

moc_VendorTypesData.cpp: VendorTypesData.h
	$(MOC) VendorTypesData.h -o moc_VendorTypesData.cpp

moc_VendorTypeEdit.cpp: VendorTypeEdit.h
	$(MOC) VendorTypeEdit.h -o moc_VendorTypeEdit.cpp

moc_VendorTypeEditData.cpp: VendorTypeEditData.h
	$(MOC) VendorTypeEditData.h -o moc_VendorTypeEditData.cpp

moc_YesNo.cpp: YesNo.h
	$(MOC) YesNo.h -o moc_YesNo.cpp

moc_YesNoData.cpp: YesNoData.h
	$(MOC) YesNoData.h -o moc_YesNoData.cpp

moc_PaymentTerms.cpp: PaymentTerms.h
	$(MOC) PaymentTerms.h -o moc_PaymentTerms.cpp

moc_PaymentTermsData.cpp: PaymentTermsData.h
	$(MOC) PaymentTermsData.h -o moc_PaymentTermsData.cpp

moc_PaymentTermsEdit.cpp: PaymentTermsEdit.h
	$(MOC) PaymentTermsEdit.h -o moc_PaymentTermsEdit.cpp

moc_PaymentTermsEditData.cpp: PaymentTermsEditData.h
	$(MOC) PaymentTermsEditData.h -o moc_PaymentTermsEditData.cpp

moc_Accounts.cpp: Accounts.h
	$(MOC) Accounts.h -o moc_Accounts.cpp

moc_AccountsData.cpp: AccountsData.h
	$(MOC) AccountsData.h -o moc_AccountsData.cpp

moc_AccountsEdit.cpp: AccountsEdit.h
	$(MOC) AccountsEdit.h -o moc_AccountsEdit.cpp

moc_AccountsEditData.cpp: AccountsEditData.h
	$(MOC) AccountsEditData.h -o moc_AccountsEditData.cpp

moc_BillingCycles.cpp: BillingCycles.h
	$(MOC) BillingCycles.h -o moc_BillingCycles.cpp

moc_BillingCyclesData.cpp: BillingCyclesData.h
	$(MOC) BillingCyclesData.h -o moc_BillingCyclesData.cpp

moc_BillingCycleEdit.cpp: BillingCycleEdit.h
	$(MOC) BillingCycleEdit.h -o moc_BillingCycleEdit.cpp

moc_BillingCycleEditData.cpp: BillingCycleEditData.h
	$(MOC) BillingCycleEditData.h -o moc_BillingCycleEditData.cpp

moc_MsgBox.cpp: MsgBox.h
	$(MOC) MsgBox.h -o moc_MsgBox.cpp

moc_MsgBoxData.cpp: MsgBoxData.h
	$(MOC) MsgBoxData.h -o moc_MsgBoxData.cpp

moc_Billables.cpp: Billables.h
	$(MOC) Billables.h -o moc_Billables.cpp

moc_BillablesData.cpp: BillablesData.h
	$(MOC) BillablesData.h -o moc_BillablesData.cpp

moc_BillableEdit.cpp: BillableEdit.h
	$(MOC) BillableEdit.h -o moc_BillableEdit.cpp

moc_BillableEditData.cpp: BillableEditData.h
	$(MOC) BillableEditData.h -o moc_BillableEditData.cpp

moc_Customers.cpp: Customers.h
	$(MOC) Customers.h -o moc_Customers.cpp

moc_CustomersData.cpp: CustomersData.h
	$(MOC) CustomersData.h -o moc_CustomersData.cpp

moc_CustomerEdit.cpp: CustomerEdit.h
	$(MOC) CustomerEdit.h -o moc_CustomerEdit.cpp

moc_CustomerEditData.cpp: CustomerEditData.h
	$(MOC) CustomerEditData.h -o moc_CustomerEditData.cpp

moc_CustTab_ContactInfo.cpp: CustTab_ContactInfo.h
	$(MOC) CustTab_ContactInfo.h -o moc_CustTab_ContactInfo.cpp

moc_CustTab_ContactInfoData.cpp: CustTab_ContactInfoData.h
	$(MOC) CustTab_ContactInfoData.h -o moc_CustTab_ContactInfoData.cpp

moc_CustTab_BillingInfo.cpp: CustTab_BillingInfo.h
	$(MOC) CustTab_BillingInfo.h -o moc_CustTab_BillingInfo.cpp

moc_CustTab_BillingInfoData.cpp: CustTab_BillingInfoData.h
	$(MOC) CustTab_BillingInfoData.h -o moc_CustTab_BillingInfoData.cpp

moc_CustTab_Logins.cpp: CustTab_Logins.h
	$(MOC) CustTab_Logins.h -o moc_CustTab_Logins.cpp

moc_CustTab_LoginsData.cpp: CustTab_LoginsData.h
	$(MOC) CustTab_LoginsData.h -o moc_CustTab_LoginsData.cpp

moc_SubscriptionEdit.cpp: SubscriptionEdit.h
	$(MOC) SubscriptionEdit.h -o moc_SubscriptionEdit.cpp

moc_SubscriptionEditData.cpp: SubscriptionEditData.h
	$(MOC) SubscriptionEditData.h -o moc_SubscriptionEditData.cpp

moc_Report_ModemUsage.cpp: Report_ModemUsage.h
	$(MOC) Report_ModemUsage.h -o moc_Report_ModemUsage.cpp

moc_Report_ModemUsageData.cpp: Report_ModemUsageData.h
	$(MOC) Report_ModemUsageData.h -o moc_Report_ModemUsageData.cpp

moc_ProgressWindow.cpp: ProgressWindow.h
	$(MOC) ProgressWindow.h -o moc_ProgressWindow.cpp

moc_ProgressWindowData.cpp: ProgressWindowData.h
	$(MOC) ProgressWindowData.h -o moc_ProgressWindowData.cpp

moc_CustTab_Notes.cpp: CustTab_Notes.h
	$(MOC) CustTab_Notes.h -o moc_CustTab_Notes.cpp

moc_CustTab_NotesData.cpp: CustTab_NotesData.h
	$(MOC) CustTab_NotesData.h -o moc_CustTab_NotesData.cpp

moc_NoteEdit.cpp: NoteEdit.h
	$(MOC) NoteEdit.h -o moc_NoteEdit.cpp

moc_NoteEditData.cpp: NoteEditData.h
	$(MOC) NoteEditData.h -o moc_NoteEditData.cpp

moc_LoginEdit.cpp: LoginEdit.h
	$(MOC) LoginEdit.h -o moc_LoginEdit.cpp

moc_LoginEditData.cpp: LoginEditData.h
	$(MOC) LoginEditData.h -o moc_LoginEditData.cpp

moc_Vendors.cpp: Vendors.h
	$(MOC) Vendors.h -o moc_Vendors.cpp

moc_VendorsData.cpp: VendorsData.h
	$(MOC) VendorsData.h -o moc_VendorsData.cpp

moc_VendorEdit.cpp: VendorEdit.h
	$(MOC) VendorEdit.h -o moc_VendorEdit.cpp

moc_VendorEditData.cpp: VendorEditData.h
	$(MOC) VendorEditData.h -o moc_VendorEditData.cpp

moc_RatePlans.cpp: RatePlans.h
	$(MOC) RatePlans.h -o moc_RatePlans.cpp

moc_RatePlansData.cpp: RatePlansData.h
	$(MOC) RatePlansData.h -o moc_RatePlansData.cpp

moc_RatePlanEdit.cpp: RatePlanEdit.h
	$(MOC) RatePlanEdit.h -o moc_RatePlanEdit.cpp

moc_RatePlanEditData.cpp: RatePlanEditData.h
	$(MOC) RatePlanEditData.h -o moc_RatePlanEditData.cpp

moc_BillableItemEdit.cpp: BillableItemEdit.h
	$(MOC) BillableItemEdit.h -o moc_BillableItemEdit.cpp

moc_BillableItemEditData.cpp: BillableItemEditData.h
	$(MOC) BillableItemEditData.h -o moc_BillableItemEditData.cpp

moc_GenJournalEntry.cpp: GenJournalEntry.h
	$(MOC) GenJournalEntry.h -o moc_GenJournalEntry.cpp

moc_GenJournalEntryData.cpp: GenJournalEntryData.h
	$(MOC) GenJournalEntryData.h -o moc_GenJournalEntryData.cpp

# DO NOT DELETE THIS LINE -- make depend depends on it.
