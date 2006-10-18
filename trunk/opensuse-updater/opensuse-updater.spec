#
# spec file for package opensuse-updater (Version 0.1.svn.r390)
#
# Copyright (c) 2006 SUSE LINUX Products GmbH, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

%define svn_r 410
%define maj_r 0.3
 
Name:           opensuse-updater
BuildRequires:  kdelibs3-devel update-desktop-files
License:        GPL
Group:          System/Management
Summary:        KDE updater applet for OpenSUSE
Version:        %{maj_r}.svn.r%{svn_r}
Release:        1
URL:            http://en.opensuse.org/KDE_Updater_Applet
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Source0:        opensuseupdater-%{maj_r}.tar.gz

%description
KDE updater applet for OpenSUSE

Authors:
--------
    Narayan Newton <narayannewton@gmail.com>
    Duncan Mac-Vicar <dmacvicar@novell.com>
    Martin Vidner <mvidner@novell.com>

%prep
%setup -q -n opensuseupdater-%{maj_r} 
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
%suse_update_desktop_file opensuseupdater

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
%dir /opt/kde3/share/apps/opensuseupdater/
%dir /opt/kde3/share/apps/opensuseupdater/pics
/opt/kde3/share/apps/opensuseupdater/pics/*
/opt/kde3/share/icons/hicolor/*
/opt/kde3/share/applnk/*
/opt/kde3/share/autostart/*.desktop
/opt/kde3/share/applications/kde/opensuseupdater.desktop
/opt/kde3/%{_lib}/*
/opt/kde3/share/doc/*
/opt/kde3/share/services/*

%changelog -n opensuse-updater
* Wed Aug 30 2006 - stbinner@suse.de
- fix build
* Tue Aug 29 2006 - dmacvicar@suse.de
- revision 390
