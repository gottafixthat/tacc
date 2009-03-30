####### This section is automatically generated

USEDES = 1

INCDIR	=  $(QTDIR)/include
CFLAGS= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I. -I../include -I../lib -I/usr/local/include
CFLAGS += -I../reports -I../voicemail -I../tickets -I../adb -I../ccards
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

ifdef EXPERIMENTAL
	CFLAGS	+= -DEXPERIMENTAL
endif

####### End of automatically generated section

####### Files

SUBDIRS =	

SOURCES =	Customers.cpp NewCustomer.cpp\
		EditCustomer.cpp\
		ReceivePayment.cpp\
		CustChargeEdit.cpp\
		SubscriptionEdit.cpp\
		CustRegister.cpp\
		CustomerContactEditor.cpp\
		Tab_ContactInfo.cpp\
		Tab_BillingInfo.cpp\
		CustSubscrList.cpp\
		Tab_Logins.cpp\
		Tab_Domains.cpp\
		DomainAliases.cpp\
		Tab_Notes.cpp\
		LoginEdit.cpp\
		NoteEdit.cpp\
		CCEntry.cpp\
		CCRefund.cpp\
		ChangePassword.cpp\
		DomainAdd.cpp\
		DomainChecklist.cpp\
		ChangeRatePlan.cpp\
		ChangeBillingCycle.cpp\
		StatementView.cpp\
		TransferLogin.cpp\
		ExtendGraceDate.cpp\
		LogCall.cpp

ifdef EXPERIMENTAL
	SOURCES	+= Tab_VoIP.cpp
endif



HEADERS =	Customers.h NewCustomer.h\
		CustRegister.h\
		EditCustomer.h\
		ReceivePayment.h\
		CustChargeEdit.h\
		SubscriptionEdit.h\
		CustomerContactEditor.h\
		Tab_ContactInfo.h\
		Tab_BillingInfo.h\
		CustSubscrList.h\
		Tab_Logins.h\
		Tab_Domains.h\
		DomainAliases.h\
		Tab_Notes.h\
		LoginEdit.h\
		NoteEdit.h\
		CCEntry.h\
		CCRefund.h\
		ChangePassword.h\
		DomainAdd.h\
		DomainChecklist.h\
		ChangeRatePlan.h\
		ChangeBillingCycle.h\
		StatementView.h\
		TransferLogin.h\
		ExtendGraceDate.h\
		LogCall.h

ifdef EXPERIMENTAL
	HEADERS	+= Tab_VoIP.h
endif

OBJECTS =	$(SOURCES:.cpp=.o)

SRCMETA =	moc_Customers.cpp \
		moc_NewCustomer.cpp\
		moc_CustRegister.cpp\
		moc_EditCustomer.cpp\
		moc_ReceivePayment.cpp\
		moc_CustChargeEdit.cpp\
		moc_SubscriptionEdit.cpp\
		moc_CustomerContactEditor.cpp\
		moc_Tab_ContactInfo.cpp\
		moc_Tab_BillingInfo.cpp\
		moc_CustSubscrList.cpp\
		moc_Tab_Logins.cpp\
		moc_Tab_Domains.cpp\
		moc_DomainAliases.cpp\
		moc_Tab_Notes.cpp\
		moc_LoginEdit.cpp\
		moc_NoteEdit.cpp\
		moc_CCEntry.cpp\
		moc_CCRefund.cpp\
		moc_ChangePassword.cpp\
		moc_DomainAdd.cpp\
		moc_DomainChecklist.cpp\
		moc_ChangeRatePlan.cpp\
		moc_ChangeBillingCycle.cpp\
		moc_StatementView.cpp\
		moc_TransferLogin.cpp\
		moc_ExtendGraceDate.cpp\
		moc_LogCall.cpp

ifdef EXPERIMENTAL
	SRCMETA	+= moc_Tab_VoIP.cpp
endif

OBJMETA =	$(SRCMETA:.cpp=.o)

TARGET	=	../lib/libcustomers.a

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	echo Compiling  $<
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(SRCMETA) $(OBJMETA) cust-lib

cust-lib: $(OBJECTS)
	ar r $(TARGET) $(OBJECTS) $(OBJMETA)
	ranlib $(TARGET)
	rm -f ../bin/tacc

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

moc_Customers.cpp: Customers.h
	$(MOC) Customers.h -o moc_Customers.cpp

moc_NewCustomer.cpp: NewCustomer.h
	$(MOC) NewCustomer.h -o moc_NewCustomer.cpp

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

moc_CustomerContactEditor.cpp: CustomerContactEditor.h
	$(MOC) CustomerContactEditor.h -o moc_CustomerContactEditor.cpp

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

moc_LoginEdit.cpp: LoginEdit.h
	$(MOC) LoginEdit.h -o moc_LoginEdit.cpp

moc_NoteEdit.cpp: NoteEdit.h
	$(MOC) NoteEdit.h -o moc_NoteEdit.cpp

moc_CCEntry.cpp: CCEntry.h
	$(MOC) CCEntry.h -o moc_CCEntry.cpp

moc_CCRefund.cpp: CCRefund.h
	$(MOC) CCRefund.h -o moc_CCRefund.cpp

moc_ChangePassword.cpp: ChangePassword.h
	$(MOC) ChangePassword.h -o moc_ChangePassword.cpp

moc_DomainAdd.cpp: DomainAdd.h
	$(MOC) DomainAdd.h -o moc_DomainAdd.cpp

moc_DomainChecklist.cpp: DomainChecklist.h
	$(MOC) DomainChecklist.h -o moc_DomainChecklist.cpp

moc_ChangeRatePlan.cpp: ChangeRatePlan.h
	$(MOC) ChangeRatePlan.h -o moc_ChangeRatePlan.cpp

moc_ChangeBillingCycle.cpp: ChangeBillingCycle.h
	$(MOC) ChangeBillingCycle.h -o moc_ChangeBillingCycle.cpp

moc_StatementView.cpp: StatementView.h
	$(MOC) StatementView.h -o moc_StatementView.cpp

moc_TransferLogin.cpp: TransferLogin.h
	$(MOC) TransferLogin.h -o moc_TransferLogin.cpp

moc_ExtendGraceDate.cpp: ExtendGraceDate.h
	$(MOC) ExtendGraceDate.h -o moc_ExtendGraceDate.cpp

moc_LogCall.cpp: LogCall.h
	$(MOC) LogCall.h -o moc_LogCall.cpp

moc_CustomerPicker.cpp: CustomerPicker.h
	$(MOC) CustomerPicker.h -o moc_CustomerPicker.cpp

moc_CustomerPickerData.cpp: CustomerPickerData.h
	$(MOC) CustomerPickerData.h -o moc_CustomerPickerData.cpp






# DO NOT DELETE THIS LINE -- make depend depends on it.
