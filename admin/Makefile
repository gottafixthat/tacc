####### This section is automatically generated

USEDES = 1

INCDIRS	=  -I$(QTDIR)/include -I. -I../customers -I../include -I../lib
INCDIRS += -I../ccards
INCDIRS += -I../reports -I../config -I../voicemail -I../adb -I../bdes
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include
CFLAGS += -DADBQT
# CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
LFLAGS  += -lcustomers -lccards -L../reports -lreports -lconfig -lvoicemail -lscheduler
LFLAGS  += -ladbqt -lqt -lmysqlclient -lblarg -lcrypt -L../lib -L$(QTDIR)/lib 
LFLAGS	+= -L/usr/X11R6/lib -lX11 -lXext -L/usr/lib/mysql -lmysqlclient
LFLAGS	+= -L../adb ../bdes
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

SOURCES =	GLAccountTypes.cpp\
		ChartOfAccounts.cpp\
		GLAccountEditor.cpp\
		RatePlans.cpp\
		RatePlanEdit.cpp\
		BillingCycles.cpp\
		BillingCycleEdit.cpp\
		VendorTypes.cpp\
		VendorTypeEdit.cpp\
		Vendors.cpp\
		VendorEdit.cpp\
		PaymentTerms.cpp\
		PaymentTermsEdit.cpp\
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
		TemplateEditor.cpp\
		DomainTypeEdit.cpp\
		DomainTypeBillablesAdd.cpp DomainTypeBillablesAddData.cpp\
		DNS_Templates.cpp\
		OverdueAccounts.cpp\
		WipedAccounts.cpp WipedAccountsData.cpp\
		RunSubscriptions.cpp\
        TE_Main.cpp\
        TE_Message.cpp\
        TE_LoginTypes.cpp\
        TE_Cities.cpp\
		MakeDeposits.cpp\
        UserEditor.cpp UserPrivs.cpp\
		CompanyEditor.cpp\
		LoginFlagList.cpp

HEADERS =	GLAccountTypes.h\
		ChartOfAccounts.h\
		GLAccountEditor.h\
		RatePlans.h\
		RatePlanEdit.h\
		BillingCycles.h\
		BillingCycleEdit.h\
		VendorTypes.h\
		VendorTypeEdit.h\
		Vendors.h\
		VendorEdit.h\
		PaymentTerms.h\
		PaymentTermsEdit.h\
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
		TemplateEditor.h\
		DomainTypeEdit.h\
		DomainTypeBillablesAdd.h DomainTypeBillablesAddData.h\
		DNS_Templates.h\
		OverdueAccounts.h\
		WipedAccounts.h WipedAccountsData.h\
		RunSubscriptions.h\
		MakeDeposits.h\
		TE_Main.h\
		TE_Message.h\
		TE_LoginTypes.h\
		TE_Cities.h\
		UserEditor.h UserPrivs.h\
		CompanyEditor.h\
		LoginFlagList.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_GLAccountTypes.cpp\
		moc_ChartOfAccounts.cpp\
		moc_GLAccountEditor.cpp\
		moc_RatePlans.cpp\
		moc_RatePlanEdit.cpp\
		moc_BillingCycles.cpp\
		moc_BillingCycleEdit.cpp\
		moc_VendorTypes.cpp\
		moc_VendorTypeEdit.cpp\
		moc_Vendors.cpp\
		moc_VendorEdit.cpp\
		moc_PaymentTerms.cpp\
		moc_PaymentTermsEdit.cpp\
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
		moc_TemplateEditor.cpp\
		moc_DomainTypeEdit.cpp\
		moc_DomainTypeBillablesAdd.cpp moc_DomainTypeBillablesAddData.cpp\
		moc_DNS_Templates.cpp\
		moc_OverdueAccounts.cpp\
		moc_WipedAccounts.cpp moc_WipedAccountsData.cpp\
		moc_RunSubscriptions.cpp\
		moc_MakeDeposits.cpp\
		moc_TE_Main.cpp\
		moc_TE_Message.cpp\
		moc_TE_LoginTypes.cpp\
		moc_TE_Cities.cpp\
		moc_UserEditor.cpp moc_UserPrivs.cpp\
		moc_CompanyEditor.cpp\
		moc_LoginFlagList.cpp

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

#depend:
#	@makedepend -I$(INCDIR) $(SOURCES) 2>/dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_VendorTypes.cpp: VendorTypes.h
	$(MOC) VendorTypes.h -o moc_VendorTypes.cpp

moc_VendorTypeEdit.cpp: VendorTypeEdit.h
	$(MOC) VendorTypeEdit.h -o moc_VendorTypeEdit.cpp

moc_Vendors.cpp: Vendors.h
	$(MOC) Vendors.h -o moc_Vendors.cpp

moc_VendorEdit.cpp: VendorEdit.h
	$(MOC) VendorEdit.h -o moc_VendorEdit.cpp

moc_PaymentTerms.cpp: PaymentTerms.h
	$(MOC) PaymentTerms.h -o moc_PaymentTerms.cpp

moc_PaymentTermsEdit.cpp: PaymentTermsEdit.h
	$(MOC) PaymentTermsEdit.h -o moc_PaymentTermsEdit.cpp

moc_BillingCycles.cpp: BillingCycles.h
	$(MOC) BillingCycles.h -o moc_BillingCycles.cpp

moc_BillingCycleEdit.cpp: BillingCycleEdit.h
	$(MOC) BillingCycleEdit.h -o moc_BillingCycleEdit.cpp

moc_RatePlans.cpp: RatePlans.h
	$(MOC) RatePlans.h -o moc_RatePlans.cpp

moc_RatePlanEdit.cpp: RatePlanEdit.h
	$(MOC) RatePlanEdit.h -o moc_RatePlanEdit.cpp

moc_GLAccountTypes.cpp: GLAccountTypes.h
	$(MOC) GLAccountTypes.h -o moc_GLAccountTypes.cpp

moc_ChartOfAccounts.cpp: ChartOfAccounts.h
	$(MOC) ChartOfAccounts.h -o moc_ChartOfAccounts.cpp

moc_GLAccountEditor.cpp: GLAccountEditor.h
	$(MOC) GLAccountEditor.h -o moc_GLAccountEditor.cpp

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

moc_TemplateEditor.cpp: TemplateEditor.h
	$(MOC) TemplateEditor.h -o moc_TemplateEditor.cpp

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

moc_WipedAccounts.cpp: WipedAccounts.h
	$(MOC) WipedAccounts.h -o moc_WipedAccounts.cpp

moc_WipedAccountsData.cpp: WipedAccountsData.h
	$(MOC) WipedAccountsData.h -o moc_WipedAccountsData.cpp

moc_RunSubscriptions.cpp: RunSubscriptions.h
	$(MOC) RunSubscriptions.h -o moc_RunSubscriptions.cpp

moc_MakeDeposits.cpp: MakeDeposits.h
	$(MOC) MakeDeposits.h -o moc_MakeDeposits.cpp

moc_CompanyEditor.cpp: CompanyEditor.h
	$(MOC) CompanyEditor.h -o moc_CompanyEditor.cpp

moc_LoginFlagList.cpp: LoginFlagList.h
	$(MOC) LoginFlagList.h -o moc_LoginFlagList.cpp

moc_TE_Main.cpp: TE_Main.h
	$(MOC) TE_Main.h -o moc_TE_Main.cpp

moc_TE_Message.cpp: TE_Message.h
	$(MOC) TE_Message.h -o moc_TE_Message.cpp

moc_TE_LoginTypes.cpp: TE_LoginTypes.h
	$(MOC) TE_LoginTypes.h -o moc_TE_LoginTypes.cpp

moc_TE_Cities.cpp: TE_Cities.h
	$(MOC) TE_Cities.h -o moc_TE_Cities.cpp

moc_UserEditor.cpp: UserEditor.h
	$(MOC) UserEditor.h -o moc_UserEditor.cpp

moc_UserPrivs.cpp: UserPrivs.h
	$(MOC) UserPrivs.h -o moc_UserPrivs.cpp


# DO NOT DELETE THIS LINE -- make depend depends on it.
