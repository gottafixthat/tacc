Summary: Total Accountability Customer Care
Name: tacc
Version: {VERSION}
Release: {RELEASE}%{dist}
License: Proprietary, Use Restricted.  (C)opyright 1997-2009 Avvatel Corporation
Group: Applications/Accounting
Source: tacc-{VERSION}-{RELEASE}.tgz
Buildroot: /var/tmp/tacc
BuildRequires: mimetic-devel
Requires: mimetic

%define prefix /usr/local

%description
Total Accountability Customer Care

%prep
%setup -q

#%build
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
mkdir -p -m 755 $RPM_BUILD_ROOT%{prefix}/bin
mkdir -p -m 755 $RPM_BUILD_ROOT%{prefix}/etc
mkdir -p -m 755 $RPM_BUILD_ROOT%{prefix}/lib/taa/customers
mkdir -p -m 755 $RPM_BUILD_ROOT%{prefix}/lib/taa/icons
mkdir -p -m 755 $RPM_BUILD_ROOT%{prefix}/lib/taa/reports
mkdir -p -m 755 $RPM_BUILD_ROOT%{prefix}/lib/taa/templates/customers
mkdir -p -m 755 $RPM_BUILD_ROOT%{prefix}/lib/taa/templates/tickets
mkdir -p -m 755 $RPM_BUILD_ROOT%{prefix}/lib/taa/tickets
mkdir -p -m 755 $RPM_BUILD_ROOT%{prefix}/sbin

# Our binaries
install -m 755 bin/tacc $RPM_BUILD_ROOT%{prefix}/bin/
install -m 755 bin/cccimport $RPM_BUILD_ROOT%{prefix}/sbin/
install -m 755 bin/ccexpired $RPM_BUILD_ROOT%{prefix}/sbin/
install -m 755 bin/ccget $RPM_BUILD_ROOT%{prefix}/sbin/
install -m 755 bin/custstats $RPM_BUILD_ROOT%{prefix}/sbin/
install -m 755 bin/modembilling $RPM_BUILD_ROOT%{prefix}/sbin/
install -m 755 bin/qwestconnect $RPM_BUILD_ROOT%{prefix}/sbin/
install -m 755 bin/radimport $RPM_BUILD_ROOT%{prefix}/sbin/
install -m 755 bin/taamail $RPM_BUILD_ROOT%{prefix}/sbin/
install -m 755 bin/upgradetacc $RPM_BUILD_ROOT%{prefix}/sbin/
install -m 755 bin/wasync $RPM_BUILD_ROOT%{prefix}/sbin/


# Our sample configuration files
install -m 644 conf/taa.cf $RPM_BUILD_ROOT%{prefix}/etc/
install -m 644 conf/cccimport.cf $RPM_BUILD_ROOT%{prefix}/etc/
install -m 644 conf/ccexpired.cf $RPM_BUILD_ROOT%{prefix}/etc/
install -m 644 conf/ccget.cf $RPM_BUILD_ROOT%{prefix}/etc/
install -m 644 conf/modembilling.cf $RPM_BUILD_ROOT%{prefix}/etc/
install -m 644 conf/radimport.cf $RPM_BUILD_ROOT%{prefix}/etc/
install -m 644 conf/wasync.cf $RPM_BUILD_ROOT%{prefix}/etc/

# Our templates, etc.
install -m 644 conf/AdminEmail.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/AdminEmailCCC.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/AUPInfo.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/BillingTerms.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/Welcome.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/ccexpired.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/domain-template.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/ReminderFooter.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/ReminderHeader.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/StatementFooter.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/StatementHeader.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/statement.tex $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/statement.tex $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/cheetah-logo.eps $RPM_BUILD_ROOT%{prefix}/lib/taa/
install -m 644 conf/customers/NoteDetail.bhtml $RPM_BUILD_ROOT%{prefix}/lib/taa/customers/
install -m 644 conf/icons/phone.xpm $RPM_BUILD_ROOT%{prefix}/lib/taa/icons/
install -m 644 conf/icons/customers.xpm $RPM_BUILD_ROOT%{prefix}/lib/taa/icons/
install -m 644 conf/icons/ticket.xpm $RPM_BUILD_ROOT%{prefix}/lib/taa/icons/
install -m 644 conf/reports/LoginCount.tmpl $RPM_BUILD_ROOT%{prefix}/lib/taa/reports/
install -m 644 conf/templates/CCDeclined.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/templates/
install -m 644 conf/templates/CCInvalid.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/templates/
install -m 644 conf/templates/CCReceipt.txt $RPM_BUILD_ROOT%{prefix}/lib/taa/templates/
install -m 644 conf/templates/customers/NoteDetail.bhtml $RPM_BUILD_ROOT%{prefix}/lib/taa/templates/customers/
install -m 644 conf/templates/tickets/History.bhtml $RPM_BUILD_ROOT%{prefix}/lib/taa/templates/tickets/
install -m 644 conf/templates/tickets/ticketstatus.tmpl $RPM_BUILD_ROOT%{prefix}/lib/taa/templates/tickets/
install -m 644 conf/tickets/History.bhtml $RPM_BUILD_ROOT%{prefix}/lib/taa/tickets/
install -m 644 conf/tickets/ticketstatus.tmpl $RPM_BUILD_ROOT%{prefix}/lib/taa/tickets/


# Last but not least, our configuration files
#install -m 640 etc/cvp.ini $RPM_BUILD_ROOT/%{cvpconfigdir}
#install -m 640 etc/signup_accountinfo.xml $RPM_BUILD_ROOT/%{cvpconfigdir}

# Install our asterisk agi scripts
#install -m 755 agi/*.agi $RPM_BUILD_ROOT/var/lib/asterisk/agi-bin/

%files
%defattr (-,root,root)
# Specifying the directories will include *all* files that we installed
# above.
%{prefix}/bin
%{prefix}/sbin

# Config files
%attr(0644, root, root) %config(noreplace) %{prefix}/etc
%attr(0644, root, root) %config(noreplace) %{prefix}/lib/taa

