Requires: zmd >= 7.1.1.0-66 
BuildRequires: kdelibs3-devel update-desktop-files

Name:         opensuse-updater 
License:      GPL
Group:        System
Summary:      Updater applet for OpenSUSE / ZMD
Version:      0.2
Release:      1.0
URL:          http://en.opensuse.org/KDE_Updater_Applet
BuildRoot:    %{_tmppath}/%{name}-%{version}-build
Source0:      opensuse-updater-%{version}.tar.gz

%description
Updater applet for OpenSUSE / ZMD

Authors:
--------
    Narayan Newton <narayannewton@gmail.com>
    Duncan Mac-Vicar <dmacvicar@novell.com>
    Martin Vidner <mvidner@novell.com>
%debug_package
%prep
%setup -q -n opensuse-updater-%{version}
. /etc/opt/kde3/common_options
update_admin --no-unsermake

%build
. /etc/opt/kde3/common_options
./configure $configkde --disable-final
make

%install
. /etc/opt/kde3/common_options
make DESTDIR=$RPM_BUILD_ROOT $INSTALL_TARGET

rm -rf $RPM_BUILD_ROOT/opt/kde3/include
#%suse_update_desktop_file opensuse-updater 
#%find_lang %name

%post
%run_ldconfig

%postun
%run_ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

#%files -f %name.lang
%files
%defattr(-,root,root)
/opt/kde3/bin/*
/opt/kde3/share/apps/opensuse-updater/pics/*
/opt/kde3/share/icons/hicolor/*
/opt/kde3/share/applnk/*
/opt/kde3/share/autostart/*.desktop
/opt/kde3/share/applications/kde/opensuse-updater.desktop
/opt/kde3/lib/libkxmlrpcclient.la
/opt/kde3/lib/libkxmlrpcclient.so
/opt/kde3/lib/libkxmlrpcclient.so.0
/opt/kde3/lib/libkxmlrpcclient.so.0.0.0
/opt/kde3/share/doc/HTML/en/suseupdater/common
/opt/kde3/share/doc/HTML/en/suseupdater/index.cache.bz2
/opt/kde3/share/doc/HTML/en/suseupdater/index.docbook
/opt/kde3/lib/*
/opt/kde3/lib/kde3/kio_udshttp.la
/opt/kde3/lib/kde3/kio_udshttp.so
/opt/kde3/share/services/udshttp.protocol

%changelog -n opensuse-updater
* Thu Aug 17 2006 - narayannewton@gmail.com
- Update for uds ioslave
* Wed Jul 5 2006 - dmacvicar@suse.de
- revision 179
* Mon Jul 3 2006 - dmacvicar@suse.de
- initial package from svn

