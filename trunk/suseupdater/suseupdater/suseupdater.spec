#
# spec file for SUSE Updater (Version 0.0.1)
#
# Copyright (c) 2006 Narayan Newton <narayannewton@gmail.com>.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.

Name:				suseupdater
BuildRequires:		kdelibs3-devel qt3-devel
License:			GPL
Group:				System/Management
Provides:			SUSEUpdater
Summary:			An Extensible Package Updater Applet For KDE
URL:				https://developer.berlios.de/projects/kzmd-updater/
Version:			0.0.1
Release:			1
Epoch:				1
Source:				suseupdater-%{version}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-root

%description
SUSE Updater is an updater applet designed for KDE and supporting multiple back-ends. Currently, the only one available is ZMD, but more will be coming. It is designed to be simple and easy to use.

Authors:
-------
	Narayan Newton <narayannewton@gmail.com>


%prep

%setup -q

%build

./configure --prefix=/opt/kde3

make

%install

make install prefix=$RPM_BUILD_ROOT

%clean

rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/bin/SUSEUpdater
%dir /share/apps/suseupdater
%dir /share/apps/suseupdater/pics
/share/apps/suseupdater/pics/suse_logo.png
/share/apps/suseupdater/pics/suse_green.png
/share/apps/suseupdater/pics/suse_red.png
/share/icons/hicolor/16x16/apps/suseupdater.png
/share/icons/hicolor/32x32/apps/suseupdater.png
/share/icons/hicolor/48x48/apps/suseupdater.png
/share/icons/hicolor/64x64/apps/suseupdater.png
/share/icons/hicolor/128x128/apps/suseupdater.png
/share/applnk/Utilities/suseupdater.desktop
/bin/kzmdauthutil
