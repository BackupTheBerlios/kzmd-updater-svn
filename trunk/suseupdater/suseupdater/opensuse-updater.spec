BuildRequires: kdelibs3-devel update-desktop-files

Name:         opensuse-updater
License:      GPL
Group:        System
Summary:      Updater applet for OpenSUSE / ZMD
Version:      0.1.svn.r187
Release:      2.1
URL:          http://en.opensuse.org/KDE_Updater_Applet
BuildRoot:    %{_tmppath}/%{name}-%{version}-build
Source0:      suseupdater-%{version}.tar.gz

%description
Updater applet for OpenSUSE / ZMD

Authors:
--------
    Narayan Newton <narayannewton@gmail.com>

%debug_package
%prep
%setup -q -n suseupdater-3.5.1
. /etc/opt/kde3/common_options
update_admin

%build
. /etc/opt/kde3/common_options
./configure $configkde --disable-final
make

%install
. /etc/opt/kde3/common_options
make DESTDIR=$RPM_BUILD_ROOT $INSTALL_TARGET

rm -rf $RPM_BUILD_ROOT/opt/kde3/include
#%suse_update_desktop_file suseupdater
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
/opt/kde3/share/apps/suseupdater/pics/*
/opt/kde3/share/icons/hicolor/*
/opt/kde3/share/applnk/*
/opt/kde3/share/autostart/*.desktop
/opt/kde3/share/applications/kde/suseupdater.desktop


%changelog -n opensuse-updater
* Wed Jul 5 2006 - dmacvicar@suse.de
- revision 179
* Mon Jul 3 2006 - dmacvicar@suse.de
- initial package from svn
  
