# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = zmd zypp

messages: rc.cpp
	$(EXTRACTRC) *.ui >> rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/opensuseupdater.pot

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = opensuseupdater 

# set the include path for X, qt and KDE
INCLUDES = $(all_includes) -I$(top_srcdir)/kxmlrpcclient -I$(top_srcdir)/updater/zmd -I$(top_srcdir)/updater/zypp

# the library search path. 
opensuseupdater_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
opensuseupdater_LDADD   = $(LIB_KFILE) $(top_builddir)/kxmlrpcclient/libkxmlrpcclient.la -lkxmlrpcclient $(top_builddir)/updater/zmd/libopensuseupdater_zmd.la $(top_builddir)/updater/zypp/libopensuseupdater_zypp.la

# which sources should be compiled for kzmdupdater
opensuseupdater_SOURCES = main.cpp MainWindow.cpp HeaderWidget.cpp  UpdaterApplication.cpp UpdateListItem.cpp  TrayIcon.cpp opensuseupdater_settings.kcfgc updatersettingswidget_base.ui

kde_kcfg_DATA = opensuseupdater_settings.kcfg

# these are the headers for your project that won't be installed
noinst_HEADERS = MainWindow.h HeaderWidget.h Updater.h UpdaterApplication.h UpdaterCapabilities.h UpdateListItem.h  TrayIcon.h


# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO
KDE_ICON = AUTO

# this is where the desktop file will go 
desktopdir   = $(kde_appsdir)/Utilities
desktop_DATA = opensuseupdater.desktop

install-data-local:
	$(mkinstalldirs) $(DESTDIR)/$(prefix)/bin
	$(INSTALL_DATA) $(srcdir)/zmd/kzmd-auth-user.sh \
		$(DESTDIR)/$(prefix)/bin/kzmd-auth-user.sh
	$(mkinstalldirs) $(DESTDIR)/$(prefix)/share/autostart
	$(INSTALL_DATA) $(srcdir)/opensusestart.desktop \
		$(DESTDIR)/$(prefix)/share/autostart/opensuseupdater.desktop


include $(top_srcdir)/admin/Doxyfile.am
