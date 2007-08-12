####### This section is automatically generated

USEDES = 1

INCDIR	=  $(QTDIR)/include
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include -I../lib -I/usr/local/include
CFLAGS += -I../reports -I../voicemail -I../tickets
CFLAGS += -DADBQT
# CFLAGS	= -g -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include/
LIBDIRS = -L/usr/local/lib/blarg -L/usr/local/lib
LFLAGS  = -L../reports -lreports
LFLAGS += $(LIBDIRS) -lqt -lmysqlclient -lblarg -laddr -L../lib -L/usr/local/lib -lqdbt -lBString
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

SOURCES =	Customers.cpp NewCustomer.cpp\
		NewCustomerData.cpp\
		EditCustomer.cpp\
		ReceivePayment.cpp\
		CustChargeEdit.cpp\
		SubscriptionEdit.cpp\
		CustRegister.cpp\
		Tab_ContactInfo.cpp\
		Tab_BillingInfo.cpp\
		CustSubscrList.cpp\
		Tab_Logins.cpp\
		Tab_Domains.cpp\
		DomainAliases.cpp\
		Tab_Notes.cpp\
		Tab_VoIP.cpp\
		Tab_Accounts.cpp Tab_AccountsData.cpp\
		LoginEdit.cpp\
		NoteEdit.cpp\
		CCEntry.cpp\
		ChangePassword.cpp ChangePasswordData.cpp\
		RunStatements.cpp\
		DomainAdd.cpp\
		DomainEdit.cpp DomainEditData.cpp\
		DomainChecklist.cpp DomainChecklistData.cpp\
		ChangeRatePlan.cpp ChangeRatePlanData.cpp\
		ChangeBillingCycle.cpp ChangeBillingCycleData.cpp\
		StatementView.cpp StatementViewData.cpp\
		TransferLogin.cpp TransferLoginData.cpp\
		ExtendGraceDate.cpp ExtendGraceDateData.cpp\
		LogCall.cpp\
		CustCallDetail.cpp CustCallDetailData.cpp\
		CustomerPicker.cpp CustomerPickerData.cpp\
		NewCustWizard.cpp NewCustWizardData.cpp\
		NCWContact.cpp NCWContactData.cpp\
		NCWAccount.cpp NCWAccountData.cpp\
		NCWBilling.cpp NCWBillingData.cpp


HEADERS =	Customers.h NewCustomer.h\
		NewCustomerData.h\
		CustRegister.h\
		EditCustomer.h\
		ReceivePayment.h\
		CustChargeEdit.h\
		SubscriptionEdit.h\
		Tab_ContactInfo.h\
		Tab_BillingInfo.h\
		CustSubscrList.h\
		Tab_Logins.h\
		Tab_Domains.h\
		DomainAliases.h\
		Tab_Notes.h\
		Tab_VoIP.h\
		Tab_Accounts.h Tab_AccountsData.h\
		LoginEdit.h\
		NoteEdit.h\
		CCEntry.h\
		ChangePassword.h ChangePasswordData.h\
		DomainAdd.h\
		DomainEdit.h DomainEditData.h\
		RunStatements.h\
		DomainChecklist.h DomainChecklistData.h\
		ChangeRatePlan.h ChangeRatePlanData.h\
		ChangeBillingCycle.h ChangeBillingCycleData.h\
		StatementView.h StatementViewData.h\
		TransferLogin.h TransferLoginData.h\
		ExtendGraceDate.h ExtendGraceDateData.h\
		LogCall.h\
		CustCallDetail.h CustCallDetailData.h\
		CustomerPicker.h CustomerPickerData.h\
		NewCustWizard.h NewCustWizardData.h\
		NCWContact.h NCWContactData.h\
		NCWAccount.h NCWAccountData.h\
		NCWBilling.h NCWBillingData.h

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_Customers.cpp \
		moc_NewCustomer.cpp moc_NewCustomerData.cpp \
		moc_CustRegister.cpp\
		moc_EditCustomer.cpp\
		moc_ReceivePayment.cpp\
		moc_CustChargeEdit.cpp\
		moc_SubscriptionEdit.cpp\
		moc_Tab_ContactInfo.cpp\
		moc_Tab_BillingInfo.cpp\
		moc_CustSubscrList.cpp\
		moc_Tab_Logins.cpp\
		moc_Tab_Domains.cpp\
		moc_DomainAliases.cpp\
		moc_Tab_Notes.cpp\
		moc_Tab_VoIP.cpp\
		moc_Tab_Accounts.cpp moc_Tab_AccountsData.cpp\
		moc_LoginEdit.cpp\
		moc_NoteEdit.cpp\
		moc_CCEntry.cpp\
		moc_ChangePassword.cpp moc_ChangePasswordData.cpp\
		moc_DomainAdd.cpp\
		moc_DomainEdit.cpp moc_DomainEditData.cpp\
		moc_RunStatements.cpp\
		moc_DomainChecklist.cpp moc_DomainChecklistData.cpp\
		moc_ChangeRatePlan.cpp moc_ChangeRatePlanData.cpp\
		moc_ChangeBillingCycle.cpp moc_ChangeBillingCycleData.cpp\
		moc_StatementView.cpp moc_StatementViewData.cpp\
		moc_TransferLogin.cpp moc_TransferLoginData.cpp\
		moc_ExtendGraceDate.cpp moc_ExtendGraceDateData.cpp\
		moc_LogCall.cpp\
		moc_CustCallDetail.cpp moc_CustCallDetailData.cpp\
		moc_CustomerPicker.cpp moc_CustomerPickerData.cpp\
		moc_NewCustWizard.cpp moc_NewCustWizardData.cpp\
		moc_NCWContact.cpp moc_NCWContactData.cpp\
		moc_NCWAccount.cpp moc_NCWAccountData.cpp\
		moc_NCWBilling.cpp moc_NCWBillingData.cpp

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libcustomers.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) cust-lib

cust-lib: $(OBJECTS)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)
	rm -f ../bin/admin ../bin/sam ../bin/taa

$(SUBDIRS): FORCE
	cd $@; $(MAKE)

$(TARGET): $(SRCMETA) $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)

FORCE:

####### Meta objects

moc_RunStatements.cpp: RunStatements.h
	$(MOC) RunStatements.h -o moc_RunStatements.cpp

moc_Customers.cpp: Customers.h
	$(MOC) Customers.h -o moc_Customers.cpp

moc_NewCustomer.cpp: NewCustomer.h
	$(MOC) NewCustomer.h -o moc_NewCustomer.cpp

moc_NewCustomerData.cpp: NewCustomerData.h
	$(MOC) NewCustomerData.h -o moc_NewCustomerData.cpp

moc_CustRegister.cpp: CustRegister.h
	$(MOC) CustRegister.h -o moc_CustRegister.cpp

moc_EditCustomer.cpp: EditCustomer.h
	$(MOC) EditCustomer.h -o moc_EditCustomer.cpp

moc_ReceivePayment.cpp: ReceivePayment.h
	$(MOC) ReceivePayment.h -o moc_ReceivePayment.cpp

moc_CustChargeEdit.cpp: CustChargeEdit.h
	$(MOC) CustChargeEdit.h -o moc_CustChargeEdit.cpp

moc_SubscriptionEdit.cpp: SubscriptionEdit.h
	$(MOC) SubscriptionEdit.h -o moc_SubscriptionEdit.cpp

moc_Tab_ContactInfo.cpp: Tab_ContactInfo.h
	$(MOC) Tab_ContactInfo.h -o moc_Tab_ContactInfo.cpp

moc_Tab_BillingInfo.cpp: Tab_BillingInfo.h
	$(MOC) Tab_BillingInfo.h -o moc_Tab_BillingInfo.cpp

moc_CustSubscrList.cpp: CustSubscrList.h
	$(MOC) CustSubscrList.h -o moc_CustSubscrList.cpp

moc_Tab_Logins.cpp: Tab_Logins.h
	$(MOC) Tab_Logins.h -o moc_Tab_Logins.cpp

moc_Tab_Domains.cpp: Tab_Domains.h
	$(MOC) Tab_Domains.h -o moc_Tab_Domains.cpp

moc_DomainAliases.cpp: DomainAliases.h
	$(MOC) DomainAliases.h -o moc_DomainAliases.cpp

moc_Tab_Notes.cpp: Tab_Notes.h
	$(MOC) Tab_Notes.h -o moc_Tab_Notes.cpp

moc_Tab_VoIP.cpp: Tab_VoIP.h
	$(MOC) Tab_VoIP.h -o moc_Tab_VoIP.cpp

moc_Tab_Accounts.cpp: Tab_Accounts.h
	$(MOC) Tab_Accounts.h -o moc_Tab_Accounts.cpp

moc_Tab_AccountsData.cpp: Tab_AccountsData.h
	$(MOC) Tab_AccountsData.h -o moc_Tab_AccountsData.cpp

moc_LoginEdit.cpp: LoginEdit.h
	$(MOC) LoginEdit.h -o moc_LoginEdit.cpp

moc_NoteEdit.cpp: NoteEdit.h
	$(MOC) NoteEdit.h -o moc_NoteEdit.cpp

moc_CCEntry.cpp: CCEntry.h
	$(MOC) CCEntry.h -o moc_CCEntry.cpp

moc_ChangePassword.cpp: ChangePassword.h
	$(MOC) ChangePassword.h -o moc_ChangePassword.cpp

moc_ChangePasswordData.cpp: ChangePasswordData.h
	$(MOC) ChangePasswordData.h -o moc_ChangePasswordData.cpp

moc_DomainAdd.cpp: DomainAdd.h
	$(MOC) DomainAdd.h -o moc_DomainAdd.cpp

moc_DomainEdit.cpp: DomainEdit.h
	$(MOC) DomainEdit.h -o moc_DomainEdit.cpp

moc_DomainEditData.cpp: DomainEditData.h
	$(MOC) DomainEditData.h -o moc_DomainEditData.cpp

moc_DomainChecklist.cpp: DomainChecklist.h
	$(MOC) DomainChecklist.h -o moc_DomainChecklist.cpp

moc_DomainChecklistData.cpp: DomainChecklistData.h
	$(MOC) DomainChecklistData.h -o moc_DomainChecklistData.cpp

moc_ChangeRatePlan.cpp: ChangeRatePlan.h
	$(MOC) ChangeRatePlan.h -o moc_ChangeRatePlan.cpp

moc_ChangeRatePlanData.cpp: ChangeRatePlanData.h
	$(MOC) ChangeRatePlanData.h -o moc_ChangeRatePlanData.cpp

moc_ChangeBillingCycle.cpp: ChangeBillingCycle.h
	$(MOC) ChangeBillingCycle.h -o moc_ChangeBillingCycle.cpp

moc_ChangeBillingCycleData.cpp: ChangeBillingCycleData.h
	$(MOC) ChangeBillingCycleData.h -o moc_ChangeBillingCycleData.cpp

moc_StatementView.cpp: StatementView.h
	$(MOC) StatementView.h -o moc_StatementView.cpp

moc_StatementViewData.cpp: StatementViewData.h
	$(MOC) StatementViewData.h -o moc_StatementViewData.cpp

moc_TransferLogin.cpp: TransferLogin.h
	$(MOC) TransferLogin.h -o moc_TransferLogin.cpp

moc_TransferLoginData.cpp: TransferLoginData.h
	$(MOC) TransferLoginData.h -o moc_TransferLoginData.cpp

moc_ExtendGraceDate.cpp: ExtendGraceDate.h
	$(MOC) ExtendGraceDate.h -o moc_ExtendGraceDate.cpp

moc_ExtendGraceDateData.cpp: ExtendGraceDateData.h
	$(MOC) ExtendGraceDateData.h -o moc_ExtendGraceDateData.cpp

moc_LogCall.cpp: LogCall.h
	$(MOC) LogCall.h -o moc_LogCall.cpp

moc_CustCallDetail.cpp: CustCallDetail.h
	$(MOC) CustCallDetail.h -o moc_CustCallDetail.cpp

moc_CustCallDetailData.cpp: CustCallDetailData.h
	$(MOC) CustCallDetailData.h -o moc_CustCallDetailData.cpp

moc_CustomerPicker.cpp: CustomerPicker.h
	$(MOC) CustomerPicker.h -o moc_CustomerPicker.cpp

moc_CustomerPickerData.cpp: CustomerPickerData.h
	$(MOC) CustomerPickerData.h -o moc_CustomerPickerData.cpp

moc_NewCustWizard.cpp: NewCustWizard.h
	$(MOC) NewCustWizard.h -o moc_NewCustWizard.cpp

moc_NewCustWizardData.cpp: NewCustWizardData.h
	$(MOC) NewCustWizardData.h -o moc_NewCustWizardData.cpp

moc_NCWContact.cpp: NCWContact.h
	$(MOC) NCWContact.h -o moc_NCWContact.cpp

moc_NCWContactData.cpp: NCWContactData.h
	$(MOC) NCWContactData.h -o moc_NCWContactData.cpp

moc_NCWAccount.cpp: NCWAccount.h
	$(MOC) NCWAccount.h -o moc_NCWAccount.cpp

moc_NCWAccountData.cpp: NCWAccountData.h
	$(MOC) NCWAccountData.h -o moc_NCWAccountData.cpp

moc_NCWBilling.cpp: NCWBilling.h
	$(MOC) NCWBilling.h -o moc_NCWBilling.cpp

moc_NCWBillingData.cpp: NCWBillingData.h
	$(MOC) NCWBillingData.h -o moc_NCWBillingData.cpp





# DO NOT DELETE THIS LINE -- make depend depends on it.
