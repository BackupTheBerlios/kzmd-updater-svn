# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS=zmd

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = opensuse-updater 

# set the include path for X, qt and KDE
INCLUDES = $(all_includes) -I$(top_srcdir)/kxmlrpcclient -I$(top_srcdir)/updater/zmd

# the library search path. 
opensuse_updater_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
opensuse_updater_LDADD   = $(LIB_KFILE) $(top_builddir)/kxmlrpcclient/libkxmlrpcclient.la -lkxmlrpcclient $(top_builddir)/updater/zmd/libzmdqt.la -lzmdqt 

# which sources should be compiled for kzmdupdater
opensuse_updater_SOURCES = main.cpp MainWindow.cpp HeaderWidget.cpp  UpdaterApplication.cpp GeneralConfigWindow.cpp UpdateListItem.cpp  TrayIcon.cpp

# these are the headers for your project that won't be installed
noinst_HEADERS = MainWindow.h HeaderWidget.h Updater.h UpdaterApplication.h GeneralConfigWindow.h UpdateListItem.h  TrayIcon.h


# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO
KDE_ICON = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kzmdupdater.pot

# this is where the desktop file will go 
desktopdir   = $(kde_appsdir)/Utilities
desktop_DATA = opensuse-updater.desktop

install-data-local:
	$(mkinstalldirs) $(DESTDIR)/$(prefix)/bin
	$(INSTALL_DATA) $(srcdir)/zmd/kzmd-auth-user.sh \
		$(DESTDIR)/$(prefix)/bin/kzmd-auth-user.sh
	$(mkinstalldirs) $(DESTDIR)/$(prefix)/share/autostart
	$(INSTALL_DATA) $(srcdir)/opensuse-start.desktop \
		$(DESTDIR)/$(prefix)/share/autostart/opensuse-updater.desktop


include $(top_srcdir)/admin/Doxyfile.am
