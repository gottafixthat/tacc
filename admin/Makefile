####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR)/include -I. -I../customers -I../include -I../lib
INCDIRS += -I../ccards
INCDIRS += -I../reports -I../config -I../voicemail
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include
CFLAGS += -DADBQT
# CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
LFLAGS  += -lcustomers -lccards -L../reports -lreports -lconfig -lvoicemail -lscheduler
LFLAGS  += -ladbqt -lqt -lmysqlclient -lblarg -lcrypt -L../lib -L$(QTDIR)/lib 
LFLAGS	+= -L/usr/X11R6/lib -lX11 -lXext -L/usr/lib/mysql -lmysqlclient
LFLAGS  += -lBTools -lFParse
LFLAGS  += -lccvs
ifdef USEQWT
LFLAGS  += -lqwt
endif
LFLAGS  += -liserver -lmimepp_util -lmimepp_net -lmimepp_core
# LFLAGS  += -static
SHELL   = /bin/sh
CC	= g++
MOC	= $(QTDIR)/bin/moc

ifdef USEDES
	CFLAGS	+= -DUSEDES -I/usr/local/include
	LFLAGS	+= -L/usr/local/lib -lbdes
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	BaseList.cpp BaseListData.cpp\
		Accounts.cpp AccountsData.cpp\
		AccountsEdit.cpp AccountsEditData.cpp\
		RatePlans.cpp RatePlansData.cpp\
		RatePlanEdit.cpp RatePlanEditData.cpp\
		BillingCycles.cpp BillingCyclesData.cpp\
		BillingCycleEdit.cpp BillingCycleEditData.cpp\
		VendorTypes.cpp VendorTypesData.cpp\
		VendorTypeEdit.cpp VendorTypeEditData.cpp\
		Vendors.cpp\
		VendorEdit.cpp VendorEditData.cpp\
		PaymentTerms.cpp PaymentTermsData.cpp\
		PaymentTermsEdit.cpp PaymentTermsEditData.cpp\
		GenJournalEntry.cpp GenJournalEntryData.cpp\
		AddBaseTypeLink.cpp AddBaseTypeLinkData.cpp\
		EditLoginTypes.cpp\
		ELT_General.cpp\
		ELT_Flags.cpp\
		ELT_Billables.cpp\
		PKG_Main.cpp\
		PKG_General.cpp\
		PKG_Conversion.cpp\
		PKG_Pricing.cpp\
		PKG_Billables.cpp\
        BI_Main.cpp\
        BI_General.cpp\
        BI_Prices.cpp\
        BI_Measurement.cpp\
		LoginTypeBillablesAdd.cpp\
		LoginTypeFlagAdd.cpp\
		DomainTypeEdit.cpp\
		DomainTypeBillablesAdd.cpp DomainTypeBillablesAddData.cpp\
		DNS_Templates.cpp\
		OverdueAccounts.cpp OverdueAccountsData.cpp\
		WipedAccounts.cpp WipedAccountsData.cpp\
		RunSubscriptions.cpp RunSubscriptionsData.cpp\
        TE_Main.cpp TE_MainData.cpp\
        TE_Message.cpp TE_MessageData.cpp\
        TE_LoginTypes.cpp TE_LoginTypesData.cpp\
        TE_Cities.cpp TE_CitiesData.cpp\
		MakeDeposits.cpp\
        UserEditor.cpp UserPrivs.cpp\
        admin.cpp

HEADERS =	BaseList.h BaseListData.h\
		Accounts.h AccountsData.h\
		AccountsEdit.h AccountsEditData.h\
		RatePlans.h RatePlansData.h\
		RatePlanEdit.h RatePlanEditData.h\
		BillingCycles.h BillingCyclesData.h\
		BillingCycleEdit.h BillingCycleEditData.h\
		VendorTypes.h VendorTypesData.h\
		VendorTypeEdit.h VendorTypeEditData.h\
		Vendors.h\
		VendorEdit.h VendorEditData.h\
		PaymentTerms.h PaymentTermsData.h\
		PaymentTermsEdit.h PaymentTermsEditData.h\
		GenJournalEntry.h GenJournalEntryData.h\
		AddBaseTypeLink.h AddBaseTypeLinkData.h\
		EditLoginTypes.h EditLoginTypesData.h\
		ELT_General.h ELT_GeneralData.h\
		ELT_Flags.h\
		ELT_Billables.h\
		PKG_Main.h\
		PKG_General.h\
		PKG_Conversion.h\
		PKG_Pricing.h\
		PKG_Billables.h\
        BI_Main.h\
        BI_General.h\
        BI_Prices.h\
        BI_Measurement.h\
		LoginTypeBillablesAdd.h\
		LoginTypeFlagAdd.h\
		DomainTypeEdit.h\
		DomainTypeBillablesAdd.h DomainTypeBillablesAddData.h\
		DNS_Templates.h\
		OverdueAccounts.h OverdueAccountsData.h\
		WipedAccounts.h WipedAccountsData.h\
		RunSubscriptions.h RunSubscriptionsData.h\
		MakeDeposits.h\
		TE_Main.h TE_MainData.h\
		TE_Message.h TE_MessageData.h\
		TE_LoginTypes.h TE_LoginTypesData.h\
		TE_Cities.h TE_CitiesData.h\
		UserEditor.h UserPrivs.h\
		admin.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_BaseList.cpp moc_BaseListData.cpp\
		moc_Accounts.cpp moc_AccountsData.cpp\
		moc_AccountsEdit.cpp moc_AccountsEditData.cpp\
		moc_RatePlans.cpp moc_RatePlansData.cpp\
		moc_RatePlanEdit.cpp moc_RatePlanEditData.cpp\
		moc_BillingCycles.cpp moc_BillingCyclesData.cpp\
		moc_BillingCycleEdit.cpp moc_BillingCycleEditData.cpp\
		moc_VendorTypes.cpp moc_VendorTypesData.cpp\
		moc_VendorTypeEdit.cpp moc_VendorTypeEditData.cpp\
		moc_Vendors.cpp\
		moc_VendorEdit.cpp moc_VendorEditData.cpp\
		moc_PaymentTerms.cpp moc_PaymentTermsData.cpp\
		moc_PaymentTermsEdit.cpp moc_PaymentTermsEditData.cpp\
		moc_GenJournalEntry.cpp moc_GenJournalEntryData.cpp\
		moc_AddBaseTypeLink.cpp moc_AddBaseTypeLinkData.cpp\
		moc_EditLoginTypes.cpp\
		moc_ELT_General.cpp\
		moc_ELT_Flags.cpp\
		moc_ELT_Billables.cpp\
		moc_PKG_Main.cpp\
		moc_PKG_General.cpp\
		moc_PKG_Conversion.cpp\
		moc_PKG_Pricing.cpp\
		moc_PKG_Billables.cpp\
		moc_BI_Main.cpp\
		moc_BI_General.cpp\
		moc_BI_Prices.cpp\
		moc_BI_Measurement.cpp\
		moc_LoginTypeBillablesAdd.cpp\
		moc_LoginTypeFlagAdd.cpp\
		moc_DomainTypeEdit.cpp\
		moc_DomainTypeBillablesAdd.cpp moc_DomainTypeBillablesAddData.cpp\
		moc_DNS_Templates.cpp\
		moc_OverdueAccounts.cpp moc_OverdueAccountsData.cpp\
		moc_WipedAccounts.cpp moc_WipedAccountsData.cpp\
		moc_RunSubscriptions.cpp moc_RunSubscriptionsData.cpp\
		moc_MakeDeposits.cpp\
		moc_TE_Main.cpp moc_TE_MainData.cpp\
		moc_TE_Message.cpp moc_TE_MessageData.cpp\
		moc_TE_LoginTypes.cpp moc_TE_LoginTypesData.cpp\
		moc_TE_Cities.cpp moc_TE_CitiesData.cpp\
		moc_UserEditor.cpp moc_UserPrivs.cpp\
		moc_admin.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libadmin.a
# TARGET	=	../bin/admin

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCDIRS) $<

####### Build rules

all: $(SUBDIRS) $(TARGET)

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)
#	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2>/dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_admin.cpp: admin.h
	$(MOC) admin.h -o moc_admin.cpp

moc_BaseList.cpp: BaseList.h
	$(MOC) BaseList.h -o moc_BaseList.cpp

moc_BaseListData.cpp: BaseListData.h
	$(MOC) BaseListData.h -o moc_BaseListData.cpp

moc_VendorTypes.cpp: VendorTypes.h
	$(MOC) VendorTypes.h -o moc_VendorTypes.cpp

moc_VendorTypesData.cpp: VendorTypesData.h
	$(MOC) VendorTypesData.h -o moc_VendorTypesData.cpp

moc_VendorTypeEdit.cpp: VendorTypeEdit.h
	$(MOC) VendorTypeEdit.h -o moc_VendorTypeEdit.cpp

moc_VendorTypeEditData.cpp: VendorTypeEditData.h
	$(MOC) VendorTypeEditData.h -o moc_VendorTypeEditData.cpp

moc_Vendors.cpp: Vendors.h
	$(MOC) Vendors.h -o moc_Vendors.cpp

moc_VendorEdit.cpp: VendorEdit.h
	$(MOC) VendorEdit.h -o moc_VendorEdit.cpp

moc_VendorEditData.cpp: VendorEditData.h
	$(MOC) VendorEditData.h -o moc_VendorEditData.cpp

moc_PaymentTerms.cpp: PaymentTerms.h
	$(MOC) PaymentTerms.h -o moc_PaymentTerms.cpp

moc_PaymentTermsData.cpp: PaymentTermsData.h
	$(MOC) PaymentTermsData.h -o moc_PaymentTermsData.cpp

moc_PaymentTermsEdit.cpp: PaymentTermsEdit.h
	$(MOC) PaymentTermsEdit.h -o moc_PaymentTermsEdit.cpp

moc_PaymentTermsEditData.cpp: PaymentTermsEditData.h
	$(MOC) PaymentTermsEditData.h -o moc_PaymentTermsEditData.cpp

moc_GenJournalEntry.cpp: GenJournalEntry.h
	$(MOC) GenJournalEntry.h -o moc_GenJournalEntry.cpp

moc_GenJournalEntryData.cpp: GenJournalEntryData.h
	$(MOC) GenJournalEntryData.h -o moc_GenJournalEntryData.cpp

moc_BillingCycles.cpp: BillingCycles.h
	$(MOC) BillingCycles.h -o moc_BillingCycles.cpp

moc_BillingCyclesData.cpp: BillingCyclesData.h
	$(MOC) BillingCyclesData.h -o moc_BillingCyclesData.cpp

moc_BillingCycleEdit.cpp: BillingCycleEdit.h
	$(MOC) BillingCycleEdit.h -o moc_BillingCycleEdit.cpp

moc_BillingCycleEditData.cpp: BillingCycleEditData.h
	$(MOC) BillingCycleEditData.h -o moc_BillingCycleEditData.cpp

moc_RatePlans.cpp: RatePlans.h
	$(MOC) RatePlans.h -o moc_RatePlans.cpp

moc_RatePlansData.cpp: RatePlansData.h
	$(MOC) RatePlansData.h -o moc_RatePlansData.cpp

moc_RatePlanEdit.cpp: RatePlanEdit.h
	$(MOC) RatePlanEdit.h -o moc_RatePlanEdit.cpp

moc_RatePlanEditData.cpp: RatePlanEditData.h
	$(MOC) RatePlanEditData.h -o moc_RatePlanEditData.cpp

moc_Accounts.cpp: Accounts.h
	$(MOC) Accounts.h -o moc_Accounts.cpp

moc_AccountsData.cpp: AccountsData.h
	$(MOC) AccountsData.h -o moc_AccountsData.cpp

moc_AccountsEdit.cpp: AccountsEdit.h
	$(MOC) AccountsEdit.h -o moc_AccountsEdit.cpp

moc_AccountsEditData.cpp: AccountsEditData.h
	$(MOC) AccountsEditData.h -o moc_AccountsEditData.cpp

moc_AddBaseTypeLink.cpp: AddBaseTypeLink.h
	$(MOC) AddBaseTypeLink.h -o moc_AddBaseTypeLink.cpp

moc_AddBaseTypeLinkData.cpp: AddBaseTypeLinkData.h
	$(MOC) AddBaseTypeLinkData.h -o moc_AddBaseTypeLinkData.cpp

moc_EditLoginTypes.cpp: EditLoginTypes.h
	$(MOC) EditLoginTypes.h -o moc_EditLoginTypes.cpp

moc_ELT_General.cpp: ELT_General.h
	$(MOC) ELT_General.h -o moc_ELT_General.cpp

moc_ELT_Flags.cpp: ELT_Flags.h
	$(MOC) ELT_Flags.h -o moc_ELT_Flags.cpp

moc_ELT_Billables.cpp: ELT_Billables.h
	$(MOC) ELT_Billables.h -o moc_ELT_Billables.cpp

moc_PKG_Main.cpp: PKG_Main.h
	$(MOC) PKG_Main.h -o moc_PKG_Main.cpp

moc_PKG_General.cpp: PKG_General.h
	$(MOC) PKG_General.h -o moc_PKG_General.cpp

moc_PKG_Conversion.cpp: PKG_Conversion.h
	$(MOC) PKG_Conversion.h -o moc_PKG_Conversion.cpp

moc_PKG_Pricing.cpp: PKG_Pricing.h
	$(MOC) PKG_Pricing.h -o moc_PKG_Pricing.cpp

moc_PKG_Billables.cpp: PKG_Billables.h
	$(MOC) PKG_Billables.h -o moc_PKG_Billables.cpp

moc_BI_Main.cpp: BI_Main.h
	$(MOC) BI_Main.h -o moc_BI_Main.cpp

moc_BI_General.cpp: BI_General.h
	$(MOC) BI_General.h -o moc_BI_General.cpp

moc_BI_Prices.cpp: BI_Prices.h
	$(MOC) BI_Prices.h -o moc_BI_Prices.cpp

moc_BI_Measurement.cpp: BI_Measurement.h
	$(MOC) BI_Measurement.h -o moc_BI_Measurement.cpp

moc_LoginTypeBillablesAdd.cpp: LoginTypeBillablesAdd.h
	$(MOC) LoginTypeBillablesAdd.h -o moc_LoginTypeBillablesAdd.cpp

moc_LoginTypeFlagAdd.cpp: LoginTypeFlagAdd.h
	$(MOC) LoginTypeFlagAdd.h -o moc_LoginTypeFlagAdd.cpp

moc_DomainTypeEdit.cpp: DomainTypeEdit.h
	$(MOC) DomainTypeEdit.h -o moc_DomainTypeEdit.cpp

moc_DomainTypeBillablesAdd.cpp: DomainTypeBillablesAdd.h
	$(MOC) DomainTypeBillablesAdd.h -o moc_DomainTypeBillablesAdd.cpp

moc_DomainTypeBillablesAddData.cpp: DomainTypeBillablesAddData.h
	$(MOC) DomainTypeBillablesAddData.h -o moc_DomainTypeBillablesAddData.cpp

moc_DNS_Templates.cpp: DNS_Templates.h
	$(MOC) DNS_Templates.h -o moc_DNS_Templates.cpp

moc_OverdueAccounts.cpp: OverdueAccounts.h
	$(MOC) OverdueAccounts.h -o moc_OverdueAccounts.cpp

moc_OverdueAccountsData.cpp: OverdueAccountsData.h
	$(MOC) OverdueAccountsData.h -o moc_OverdueAccountsData.cpp

moc_WipedAccounts.cpp: WipedAccounts.h
	$(MOC) WipedAccounts.h -o moc_WipedAccounts.cpp

moc_WipedAccountsData.cpp: WipedAccountsData.h
	$(MOC) WipedAccountsData.h -o moc_WipedAccountsData.cpp

moc_RunSubscriptions.cpp: RunSubscriptions.h
	$(MOC) RunSubscriptions.h -o moc_RunSubscriptions.cpp

moc_RunSubscriptionsData.cpp: RunSubscriptionsData.h
	$(MOC) RunSubscriptionsData.h -o moc_RunSubscriptionsData.cpp

moc_MakeDeposits.cpp: MakeDeposits.h
	$(MOC) MakeDeposits.h -o moc_MakeDeposits.cpp

moc_TE_Main.cpp: TE_Main.h
	$(MOC) TE_Main.h -o moc_TE_Main.cpp

moc_TE_MainData.cpp: TE_MainData.h
	$(MOC) TE_MainData.h -o moc_TE_MainData.cpp

moc_TE_Message.cpp: TE_Message.h
	$(MOC) TE_Message.h -o moc_TE_Message.cpp

moc_TE_MessageData.cpp: TE_MessageData.h
	$(MOC) TE_MessageData.h -o moc_TE_MessageData.cpp

moc_TE_LoginTypes.cpp: TE_LoginTypes.h
	$(MOC) TE_LoginTypes.h -o moc_TE_LoginTypes.cpp

moc_TE_LoginTypesData.cpp: TE_LoginTypesData.h
	$(MOC) TE_LoginTypesData.h -o moc_TE_LoginTypesData.cpp

moc_TE_Cities.cpp: TE_Cities.h
	$(MOC) TE_Cities.h -o moc_TE_Cities.cpp

moc_TE_CitiesData.cpp: TE_CitiesData.h
	$(MOC) TE_CitiesData.h -o moc_TE_CitiesData.cpp

moc_UserEditor.cpp: UserEditor.h
	$(MOC) UserEditor.h -o moc_UserEditor.cpp

moc_UserPrivs.cpp: UserPrivs.h
	$(MOC) UserPrivs.h -o moc_UserPrivs.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.

BaseListData.o: BaseListData.h
Accounts.o: Accounts.h AccountsData.h AccountsEdit.h AccountsEditData.h
Accounts.o: /usr/include/mysql/mysql.h /usr/include/sys/types.h
Accounts.o: /usr/include/features.h /usr/include/sys/cdefs.h
Accounts.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
Accounts.o: /usr/include/bits/wordsize.h
Accounts.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
Accounts.o: /usr/include/bits/typesizes.h /usr/include/time.h
Accounts.o: /usr/include/mysql/mysql_com.h /usr/include/mysql/mysql_version.h
Accounts.o: /usr/include/mysql/my_alloc.h /usr/include/stdlib.h
Accounts.o: /usr/include/stdio.h /usr/include/libio.h
Accounts.o: /usr/include/_G_config.h /usr/include/wchar.h
Accounts.o: /usr/include/bits/wchar.h /usr/include/gconv.h
Accounts.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
Accounts.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
AccountsData.o: AccountsData.h
AccountsEdit.o: AccountsEdit.h AccountsEditData.h /usr/include/stdlib.h
AccountsEdit.o: /usr/include/features.h /usr/include/sys/cdefs.h
AccountsEdit.o: /usr/include/gnu/stubs.h
AccountsEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
AccountsEdit.o: /usr/include/stdio.h /usr/include/bits/types.h
AccountsEdit.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
AccountsEdit.o: /usr/include/libio.h /usr/include/_G_config.h
AccountsEdit.o: /usr/include/wchar.h /usr/include/bits/wchar.h
AccountsEdit.o: /usr/include/gconv.h
AccountsEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
AccountsEdit.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
AccountsEditData.o: AccountsEditData.h
RatePlans.o: RatePlans.h RatePlansData.h RatePlanEdit.h RatePlanEditData.h
RatePlans.o: /usr/include/stdio.h /usr/include/features.h
RatePlans.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
RatePlans.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
RatePlans.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
RatePlans.o: /usr/include/bits/typesizes.h /usr/include/libio.h
RatePlans.o: /usr/include/_G_config.h /usr/include/wchar.h
RatePlans.o: /usr/include/bits/wchar.h /usr/include/gconv.h
RatePlans.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
RatePlans.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
RatePlans.o: /usr/include/stdlib.h
RatePlansData.o: RatePlansData.h
RatePlanEdit.o: RatePlanEdit.h RatePlanEditData.h /usr/include/stdio.h
RatePlanEdit.o: /usr/include/features.h /usr/include/sys/cdefs.h
RatePlanEdit.o: /usr/include/gnu/stubs.h
RatePlanEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
RatePlanEdit.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
RatePlanEdit.o: /usr/include/bits/typesizes.h /usr/include/libio.h
RatePlanEdit.o: /usr/include/_G_config.h /usr/include/wchar.h
RatePlanEdit.o: /usr/include/bits/wchar.h /usr/include/gconv.h
RatePlanEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
RatePlanEdit.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
RatePlanEdit.o: /usr/include/stdlib.h
RatePlanEditData.o: RatePlanEditData.h
BillingCycles.o: BillingCycles.h BillingCyclesData.h BillingCycleEdit.h
BillingCycles.o: BillingCycleEditData.h /usr/include/stdio.h
BillingCycles.o: /usr/include/features.h /usr/include/sys/cdefs.h
BillingCycles.o: /usr/include/gnu/stubs.h
BillingCycles.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
BillingCycles.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
BillingCycles.o: /usr/include/bits/typesizes.h /usr/include/libio.h
BillingCycles.o: /usr/include/_G_config.h /usr/include/wchar.h
BillingCycles.o: /usr/include/bits/wchar.h /usr/include/gconv.h
BillingCycles.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
BillingCycles.o: /usr/include/bits/stdio_lim.h
BillingCycles.o: /usr/include/bits/sys_errlist.h
BillingCyclesData.o: BillingCyclesData.h
BillingCycleEdit.o: BillingCycleEdit.h BillingCycleEditData.h
BillingCycleEdit.o: /usr/include/stdio.h /usr/include/features.h
BillingCycleEdit.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BillingCycleEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
BillingCycleEdit.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
BillingCycleEdit.o: /usr/include/bits/typesizes.h /usr/include/libio.h
BillingCycleEdit.o: /usr/include/_G_config.h /usr/include/wchar.h
BillingCycleEdit.o: /usr/include/bits/wchar.h /usr/include/gconv.h
BillingCycleEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
BillingCycleEdit.o: /usr/include/bits/stdio_lim.h
BillingCycleEdit.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
BillingCycleEditData.o: BillingCycleEditData.h
VendorTypes.o: VendorTypes.h VendorTypesData.h VendorTypeEdit.h
VendorTypes.o: VendorTypeEditData.h /usr/include/stdlib.h
VendorTypes.o: /usr/include/features.h /usr/include/sys/cdefs.h
VendorTypes.o: /usr/include/gnu/stubs.h
VendorTypes.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
VendorTypes.o: /usr/include/stdio.h /usr/include/bits/types.h
VendorTypes.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
VendorTypes.o: /usr/include/libio.h /usr/include/_G_config.h
VendorTypes.o: /usr/include/wchar.h /usr/include/bits/wchar.h
VendorTypes.o: /usr/include/gconv.h
VendorTypes.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
VendorTypes.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
VendorTypesData.o: VendorTypesData.h
VendorTypeEdit.o: VendorTypeEdit.h VendorTypeEditData.h /usr/include/stdio.h
VendorTypeEdit.o: /usr/include/features.h /usr/include/sys/cdefs.h
VendorTypeEdit.o: /usr/include/gnu/stubs.h
VendorTypeEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
VendorTypeEdit.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
VendorTypeEdit.o: /usr/include/bits/typesizes.h /usr/include/libio.h
VendorTypeEdit.o: /usr/include/_G_config.h /usr/include/wchar.h
VendorTypeEdit.o: /usr/include/bits/wchar.h /usr/include/gconv.h
VendorTypeEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
VendorTypeEdit.o: /usr/include/bits/stdio_lim.h
VendorTypeEdit.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
VendorTypeEditData.o: VendorTypeEditData.h
Vendors.o: Vendors.h VendorEdit.h VendorEditData.h /usr/include/stdio.h
Vendors.o: /usr/include/features.h /usr/include/sys/cdefs.h
Vendors.o: /usr/include/gnu/stubs.h
Vendors.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
Vendors.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
Vendors.o: /usr/include/bits/typesizes.h /usr/include/libio.h
Vendors.o: /usr/include/_G_config.h /usr/include/wchar.h
Vendors.o: /usr/include/bits/wchar.h /usr/include/gconv.h
Vendors.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
Vendors.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
Vendors.o: /usr/include/stdlib.h
VendorEdit.o: VendorEdit.h VendorEditData.h
VendorEditData.o: VendorEditData.h
PaymentTerms.o: PaymentTerms.h PaymentTermsData.h PaymentTermsEdit.h
PaymentTerms.o: PaymentTermsEditData.h /usr/include/stdlib.h
PaymentTerms.o: /usr/include/features.h /usr/include/sys/cdefs.h
PaymentTerms.o: /usr/include/gnu/stubs.h
PaymentTerms.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
PaymentTerms.o: /usr/include/stdio.h /usr/include/bits/types.h
PaymentTerms.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
PaymentTerms.o: /usr/include/libio.h /usr/include/_G_config.h
PaymentTerms.o: /usr/include/wchar.h /usr/include/bits/wchar.h
PaymentTerms.o: /usr/include/gconv.h
PaymentTerms.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
PaymentTerms.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
PaymentTermsData.o: PaymentTermsData.h
PaymentTermsEdit.o: PaymentTermsEdit.h PaymentTermsEditData.h
PaymentTermsEdit.o: /usr/include/stdio.h /usr/include/features.h
PaymentTermsEdit.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
PaymentTermsEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
PaymentTermsEdit.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
PaymentTermsEdit.o: /usr/include/bits/typesizes.h /usr/include/libio.h
PaymentTermsEdit.o: /usr/include/_G_config.h /usr/include/wchar.h
PaymentTermsEdit.o: /usr/include/bits/wchar.h /usr/include/gconv.h
PaymentTermsEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
PaymentTermsEdit.o: /usr/include/bits/stdio_lim.h
PaymentTermsEdit.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
PaymentTermsEditData.o: PaymentTermsEditData.h
GenJournalEntry.o: GenJournalEntry.h GenJournalEntryData.h
GenJournalEntry.o: /usr/include/unistd.h /usr/include/features.h
GenJournalEntry.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
GenJournalEntry.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
GenJournalEntry.o: /usr/include/bits/wordsize.h
GenJournalEntry.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
GenJournalEntry.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
GenJournalEntry.o: /usr/include/time.h /usr/include/stdio.h
GenJournalEntry.o: /usr/include/libio.h /usr/include/_G_config.h
GenJournalEntry.o: /usr/include/wchar.h /usr/include/bits/wchar.h
GenJournalEntry.o: /usr/include/gconv.h
GenJournalEntry.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
GenJournalEntry.o: /usr/include/bits/stdio_lim.h
GenJournalEntry.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
GenJournalEntryData.o: GenJournalEntryData.h
AddBaseTypeLink.o: AddBaseTypeLink.h AddBaseTypeLinkData.h
AddBaseTypeLink.o: /usr/include/stdlib.h /usr/include/features.h
AddBaseTypeLink.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
AddBaseTypeLink.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
AddBaseTypeLink.o: /usr/include/stdio.h /usr/include/bits/types.h
AddBaseTypeLink.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
AddBaseTypeLink.o: /usr/include/libio.h /usr/include/_G_config.h
AddBaseTypeLink.o: /usr/include/wchar.h /usr/include/bits/wchar.h
AddBaseTypeLink.o: /usr/include/gconv.h
AddBaseTypeLink.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
AddBaseTypeLink.o: /usr/include/bits/stdio_lim.h
AddBaseTypeLink.o: /usr/include/bits/sys_errlist.h
AddBaseTypeLinkData.o: AddBaseTypeLinkData.h
EditLoginTypes.o: EditLoginTypes.h /usr/include/stdio.h
EditLoginTypes.o: /usr/include/features.h /usr/include/sys/cdefs.h
EditLoginTypes.o: /usr/include/gnu/stubs.h
EditLoginTypes.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
EditLoginTypes.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
EditLoginTypes.o: /usr/include/bits/typesizes.h /usr/include/libio.h
EditLoginTypes.o: /usr/include/_G_config.h /usr/include/wchar.h
EditLoginTypes.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EditLoginTypes.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
EditLoginTypes.o: /usr/include/bits/stdio_lim.h
EditLoginTypes.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
ELT_General.o: ELT_General.h /usr/include/stdio.h /usr/include/features.h
ELT_General.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ELT_General.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
ELT_General.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
ELT_General.o: /usr/include/bits/typesizes.h /usr/include/libio.h
ELT_General.o: /usr/include/_G_config.h /usr/include/wchar.h
ELT_General.o: /usr/include/bits/wchar.h /usr/include/gconv.h
ELT_General.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
ELT_General.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
ELT_General.o: /usr/include/stdlib.h
ELT_Flags.o: ELT_Flags.h /usr/include/stdio.h /usr/include/features.h
ELT_Flags.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ELT_Flags.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
ELT_Flags.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
ELT_Flags.o: /usr/include/bits/typesizes.h /usr/include/libio.h
ELT_Flags.o: /usr/include/_G_config.h /usr/include/wchar.h
ELT_Flags.o: /usr/include/bits/wchar.h /usr/include/gconv.h
ELT_Flags.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
ELT_Flags.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
ELT_Flags.o: /usr/include/stdlib.h LoginTypeFlagAdd.h
ELT_Billables.o: ELT_Billables.h /usr/include/stdio.h /usr/include/features.h
ELT_Billables.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ELT_Billables.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
ELT_Billables.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
ELT_Billables.o: /usr/include/bits/typesizes.h /usr/include/libio.h
ELT_Billables.o: /usr/include/_G_config.h /usr/include/wchar.h
ELT_Billables.o: /usr/include/bits/wchar.h /usr/include/gconv.h
ELT_Billables.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
ELT_Billables.o: /usr/include/bits/stdio_lim.h
ELT_Billables.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
ELT_Billables.o: LoginTypeBillablesAdd.h
PKG_Main.o: PKG_Main.h PKG_General.h PKG_Conversion.h PKG_Pricing.h
PKG_Main.o: PKG_Billables.h /usr/include/stdio.h /usr/include/features.h
PKG_Main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
PKG_Main.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
PKG_Main.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
PKG_Main.o: /usr/include/bits/typesizes.h /usr/include/libio.h
PKG_Main.o: /usr/include/_G_config.h /usr/include/wchar.h
PKG_Main.o: /usr/include/bits/wchar.h /usr/include/gconv.h
PKG_Main.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
PKG_Main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
PKG_Main.o: /usr/include/stdlib.h
PKG_General.o: PKG_General.h /usr/include/stdio.h /usr/include/features.h
PKG_General.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
PKG_General.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
PKG_General.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
PKG_General.o: /usr/include/bits/typesizes.h /usr/include/libio.h
PKG_General.o: /usr/include/_G_config.h /usr/include/wchar.h
PKG_General.o: /usr/include/bits/wchar.h /usr/include/gconv.h
PKG_General.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
PKG_General.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
PKG_General.o: /usr/include/stdlib.h
PKG_Conversion.o: PKG_Conversion.h /usr/include/stdio.h
PKG_Conversion.o: /usr/include/features.h /usr/include/sys/cdefs.h
PKG_Conversion.o: /usr/include/gnu/stubs.h
PKG_Conversion.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
PKG_Conversion.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
PKG_Conversion.o: /usr/include/bits/typesizes.h /usr/include/libio.h
PKG_Conversion.o: /usr/include/_G_config.h /usr/include/wchar.h
PKG_Conversion.o: /usr/include/bits/wchar.h /usr/include/gconv.h
PKG_Conversion.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
PKG_Conversion.o: /usr/include/bits/stdio_lim.h
PKG_Conversion.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
PKG_Pricing.o: PKG_Pricing.h /usr/include/stdio.h /usr/include/features.h
PKG_Pricing.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
PKG_Pricing.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
PKG_Pricing.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
PKG_Pricing.o: /usr/include/bits/typesizes.h /usr/include/libio.h
PKG_Pricing.o: /usr/include/_G_config.h /usr/include/wchar.h
PKG_Pricing.o: /usr/include/bits/wchar.h /usr/include/gconv.h
PKG_Pricing.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
PKG_Pricing.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
PKG_Pricing.o: /usr/include/stdlib.h
PKG_Billables.o: PKG_Billables.h /usr/include/stdio.h /usr/include/features.h
PKG_Billables.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
PKG_Billables.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
PKG_Billables.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
PKG_Billables.o: /usr/include/bits/typesizes.h /usr/include/libio.h
PKG_Billables.o: /usr/include/_G_config.h /usr/include/wchar.h
PKG_Billables.o: /usr/include/bits/wchar.h /usr/include/gconv.h
PKG_Billables.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
PKG_Billables.o: /usr/include/bits/stdio_lim.h
PKG_Billables.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
BI_Main.o: BI_Main.h BI_General.h BI_Prices.h BI_Measurement.h
BI_Main.o: /usr/include/stdio.h /usr/include/features.h
BI_Main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BI_Main.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
BI_Main.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
BI_Main.o: /usr/include/bits/typesizes.h /usr/include/libio.h
BI_Main.o: /usr/include/_G_config.h /usr/include/wchar.h
BI_Main.o: /usr/include/bits/wchar.h /usr/include/gconv.h
BI_Main.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
BI_Main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
BI_Main.o: /usr/include/stdlib.h
BI_General.o: BI_General.h /usr/include/stdio.h /usr/include/features.h
BI_General.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BI_General.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
BI_General.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
BI_General.o: /usr/include/bits/typesizes.h /usr/include/libio.h
BI_General.o: /usr/include/_G_config.h /usr/include/wchar.h
BI_General.o: /usr/include/bits/wchar.h /usr/include/gconv.h
BI_General.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
BI_General.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
BI_General.o: /usr/include/stdlib.h
BI_Prices.o: BI_Prices.h /usr/include/stdio.h /usr/include/features.h
BI_Prices.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BI_Prices.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
BI_Prices.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
BI_Prices.o: /usr/include/bits/typesizes.h /usr/include/libio.h
BI_Prices.o: /usr/include/_G_config.h /usr/include/wchar.h
BI_Prices.o: /usr/include/bits/wchar.h /usr/include/gconv.h
BI_Prices.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
BI_Prices.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
BI_Prices.o: /usr/include/stdlib.h
BI_Measurement.o: BI_Measurement.h /usr/include/stdio.h
BI_Measurement.o: /usr/include/features.h /usr/include/sys/cdefs.h
BI_Measurement.o: /usr/include/gnu/stubs.h
BI_Measurement.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
BI_Measurement.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
BI_Measurement.o: /usr/include/bits/typesizes.h /usr/include/libio.h
BI_Measurement.o: /usr/include/_G_config.h /usr/include/wchar.h
BI_Measurement.o: /usr/include/bits/wchar.h /usr/include/gconv.h
BI_Measurement.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
BI_Measurement.o: /usr/include/bits/stdio_lim.h
BI_Measurement.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
LoginTypeBillablesAdd.o: LoginTypeBillablesAdd.h /usr/include/stdio.h
LoginTypeBillablesAdd.o: /usr/include/features.h /usr/include/sys/cdefs.h
LoginTypeBillablesAdd.o: /usr/include/gnu/stubs.h
LoginTypeBillablesAdd.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
LoginTypeBillablesAdd.o: /usr/include/bits/types.h
LoginTypeBillablesAdd.o: /usr/include/bits/wordsize.h
LoginTypeBillablesAdd.o: /usr/include/bits/typesizes.h /usr/include/libio.h
LoginTypeBillablesAdd.o: /usr/include/_G_config.h /usr/include/wchar.h
LoginTypeBillablesAdd.o: /usr/include/bits/wchar.h /usr/include/gconv.h
LoginTypeBillablesAdd.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
LoginTypeBillablesAdd.o: /usr/include/bits/stdio_lim.h
LoginTypeBillablesAdd.o: /usr/include/bits/sys_errlist.h
LoginTypeBillablesAdd.o: /usr/include/stdlib.h
LoginTypeFlagAdd.o: LoginTypeFlagAdd.h /usr/include/stdio.h
LoginTypeFlagAdd.o: /usr/include/features.h /usr/include/sys/cdefs.h
LoginTypeFlagAdd.o: /usr/include/gnu/stubs.h
LoginTypeFlagAdd.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
LoginTypeFlagAdd.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
LoginTypeFlagAdd.o: /usr/include/bits/typesizes.h /usr/include/libio.h
LoginTypeFlagAdd.o: /usr/include/_G_config.h /usr/include/wchar.h
LoginTypeFlagAdd.o: /usr/include/bits/wchar.h /usr/include/gconv.h
LoginTypeFlagAdd.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
LoginTypeFlagAdd.o: /usr/include/bits/stdio_lim.h
LoginTypeFlagAdd.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
DomainTypeEdit.o: DomainTypeEdit.h DomainTypeBillablesAdd.h
DomainTypeEdit.o: DomainTypeBillablesAddData.h /usr/include/stdio.h
DomainTypeEdit.o: /usr/include/features.h /usr/include/sys/cdefs.h
DomainTypeEdit.o: /usr/include/gnu/stubs.h
DomainTypeEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
DomainTypeEdit.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
DomainTypeEdit.o: /usr/include/bits/typesizes.h /usr/include/libio.h
DomainTypeEdit.o: /usr/include/_G_config.h /usr/include/wchar.h
DomainTypeEdit.o: /usr/include/bits/wchar.h /usr/include/gconv.h
DomainTypeEdit.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
DomainTypeEdit.o: /usr/include/bits/stdio_lim.h
DomainTypeEdit.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
DomainTypeBillablesAdd.o: DomainTypeBillablesAdd.h
DomainTypeBillablesAdd.o: DomainTypeBillablesAddData.h /usr/include/stdio.h
DomainTypeBillablesAdd.o: /usr/include/features.h /usr/include/sys/cdefs.h
DomainTypeBillablesAdd.o: /usr/include/gnu/stubs.h
DomainTypeBillablesAdd.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
DomainTypeBillablesAdd.o: /usr/include/bits/types.h
DomainTypeBillablesAdd.o: /usr/include/bits/wordsize.h
DomainTypeBillablesAdd.o: /usr/include/bits/typesizes.h /usr/include/libio.h
DomainTypeBillablesAdd.o: /usr/include/_G_config.h /usr/include/wchar.h
DomainTypeBillablesAdd.o: /usr/include/bits/wchar.h /usr/include/gconv.h
DomainTypeBillablesAdd.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
DomainTypeBillablesAdd.o: /usr/include/bits/stdio_lim.h
DomainTypeBillablesAdd.o: /usr/include/bits/sys_errlist.h
DomainTypeBillablesAdd.o: /usr/include/stdlib.h
DomainTypeBillablesAddData.o: DomainTypeBillablesAddData.h
DNS_Templates.o: /usr/include/stdlib.h /usr/include/features.h
DNS_Templates.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
DNS_Templates.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
DNS_Templates.o: DNS_Templates.h
OverdueAccounts.o: OverdueAccounts.h OverdueAccountsData.h
OverdueAccounts.o: /usr/include/stdio.h /usr/include/features.h
OverdueAccounts.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
OverdueAccounts.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
OverdueAccounts.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
OverdueAccounts.o: /usr/include/bits/typesizes.h /usr/include/libio.h
OverdueAccounts.o: /usr/include/_G_config.h /usr/include/wchar.h
OverdueAccounts.o: /usr/include/bits/wchar.h /usr/include/gconv.h
OverdueAccounts.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
OverdueAccounts.o: /usr/include/bits/stdio_lim.h
OverdueAccounts.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
OverdueAccounts.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
OverdueAccounts.o: /usr/include/bits/confname.h
OverdueAccountsData.o: OverdueAccountsData.h
WipedAccounts.o: WipedAccounts.h WipedAccountsData.h /usr/include/stdio.h
WipedAccounts.o: /usr/include/features.h /usr/include/sys/cdefs.h
WipedAccounts.o: /usr/include/gnu/stubs.h
WipedAccounts.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
WipedAccounts.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
WipedAccounts.o: /usr/include/bits/typesizes.h /usr/include/libio.h
WipedAccounts.o: /usr/include/_G_config.h /usr/include/wchar.h
WipedAccounts.o: /usr/include/bits/wchar.h /usr/include/gconv.h
WipedAccounts.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
WipedAccounts.o: /usr/include/bits/stdio_lim.h
WipedAccounts.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
WipedAccounts.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
WipedAccounts.o: /usr/include/bits/confname.h
WipedAccountsData.o: WipedAccountsData.h
RunSubscriptions.o: /usr/include/stdio.h /usr/include/features.h
RunSubscriptions.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
RunSubscriptions.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
RunSubscriptions.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
RunSubscriptions.o: /usr/include/bits/typesizes.h /usr/include/libio.h
RunSubscriptions.o: /usr/include/_G_config.h /usr/include/wchar.h
RunSubscriptions.o: /usr/include/bits/wchar.h /usr/include/gconv.h
RunSubscriptions.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
RunSubscriptions.o: /usr/include/bits/stdio_lim.h
RunSubscriptions.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
RunSubscriptions.o: RunSubscriptions.h RunSubscriptionsData.h
RunSubscriptionsData.o: RunSubscriptionsData.h
TE_Main.o: TE_Main.h TE_MainData.h TE_Message.h TE_MessageData.h
TE_Main.o: TE_LoginTypes.h TE_LoginTypesData.h TE_Cities.h TE_CitiesData.h
TE_Main.o: /usr/include/stdio.h /usr/include/features.h
TE_Main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
TE_Main.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
TE_Main.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TE_Main.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TE_Main.o: /usr/include/_G_config.h /usr/include/wchar.h
TE_Main.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TE_Main.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
TE_Main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
TE_Main.o: /usr/include/stdlib.h /usr/include/unistd.h
TE_Main.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
TE_MainData.o: TE_MainData.h
TE_Message.o: TE_Message.h TE_MessageData.h /usr/include/stdio.h
TE_Message.o: /usr/include/features.h /usr/include/sys/cdefs.h
TE_Message.o: /usr/include/gnu/stubs.h
TE_Message.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
TE_Message.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TE_Message.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TE_Message.o: /usr/include/_G_config.h /usr/include/wchar.h
TE_Message.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TE_Message.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
TE_Message.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
TE_Message.o: /usr/include/stdlib.h
TE_MessageData.o: TE_MessageData.h
TE_LoginTypes.o: TE_LoginTypes.h TE_LoginTypesData.h /usr/include/stdio.h
TE_LoginTypes.o: /usr/include/features.h /usr/include/sys/cdefs.h
TE_LoginTypes.o: /usr/include/gnu/stubs.h
TE_LoginTypes.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
TE_LoginTypes.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
TE_LoginTypes.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TE_LoginTypes.o: /usr/include/_G_config.h /usr/include/wchar.h
TE_LoginTypes.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TE_LoginTypes.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
TE_LoginTypes.o: /usr/include/bits/stdio_lim.h
TE_LoginTypes.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
TE_LoginTypesData.o: TE_LoginTypesData.h
TE_Cities.o: TE_Cities.h TE_CitiesData.h
TE_CitiesData.o: TE_CitiesData.h
MakeDeposits.o: /usr/include/stdio.h /usr/include/features.h
MakeDeposits.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
MakeDeposits.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
MakeDeposits.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
MakeDeposits.o: /usr/include/bits/typesizes.h /usr/include/libio.h
MakeDeposits.o: /usr/include/_G_config.h /usr/include/wchar.h
MakeDeposits.o: /usr/include/bits/wchar.h /usr/include/gconv.h
MakeDeposits.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
MakeDeposits.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
MakeDeposits.o: /usr/include/stdlib.h MakeDeposits.h
UserEditor.o: /usr/include/stdio.h /usr/include/features.h
UserEditor.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
UserEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
UserEditor.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
UserEditor.o: /usr/include/bits/typesizes.h /usr/include/libio.h
UserEditor.o: /usr/include/_G_config.h /usr/include/wchar.h
UserEditor.o: /usr/include/bits/wchar.h /usr/include/gconv.h
UserEditor.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
UserEditor.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
UserEditor.o: /usr/include/stdlib.h UserEditor.h
UserPrivs.o: /usr/include/stdio.h /usr/include/features.h
UserPrivs.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
UserPrivs.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
UserPrivs.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
UserPrivs.o: /usr/include/bits/typesizes.h /usr/include/libio.h
UserPrivs.o: /usr/include/_G_config.h /usr/include/wchar.h
UserPrivs.o: /usr/include/bits/wchar.h /usr/include/gconv.h
UserPrivs.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
UserPrivs.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
UserPrivs.o: /usr/include/stdlib.h UserPrivs.h UserEditor.h
admin.o: /usr/include/stdio.h /usr/include/features.h
admin.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
admin.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stddef.h
admin.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
admin.o: /usr/include/bits/typesizes.h /usr/include/libio.h
admin.o: /usr/include/_G_config.h /usr/include/wchar.h
admin.o: /usr/include/bits/wchar.h /usr/include/gconv.h
admin.o: /usr/lib/gcc-lib/i586-suse-linux/3.3.3/include/stdarg.h
admin.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
admin.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
admin.o: /usr/include/bits/confname.h /usr/include/pwd.h
admin.o: /usr/include/signal.h /usr/include/bits/sigset.h
admin.o: /usr/include/bits/signum.h /usr/include/sys/types.h
admin.o: /usr/include/time.h admin.h Accounts.h AccountsData.h
admin.o: BillingCycles.h BillingCyclesData.h RatePlans.h RatePlansData.h
admin.o: VendorTypes.h VendorTypesData.h PaymentTerms.h PaymentTermsData.h
admin.o: GenJournalEntry.h GenJournalEntryData.h DomainTypeEdit.h Vendors.h
admin.o: OverdueAccounts.h OverdueAccountsData.h WipedAccounts.h
admin.o: WipedAccountsData.h RunSubscriptions.h RunSubscriptionsData.h
admin.o: EditLoginTypes.h PKG_Main.h PKG_General.h PKG_Conversion.h
admin.o: PKG_Pricing.h PKG_Billables.h BI_Main.h BI_General.h BI_Prices.h
admin.o: BI_Measurement.h MakeDeposits.h TE_Main.h TE_MainData.h TE_Message.h
admin.o: TE_MessageData.h TE_LoginTypes.h TE_LoginTypesData.h TE_Cities.h
admin.o: TE_CitiesData.h
