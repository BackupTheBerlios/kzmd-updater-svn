# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
#SUBDIRS =

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = SUSEUpdater 

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# the library search path. 
SUSEUpdater_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
SUSEUpdater_LDADD   = $(LIB_KFILE)

# which sources should be compiled for kzmdupdater
SUSEUpdater_SOURCES = main.cpp  MainWindow.cpp HeaderWidget.cpp ZmdConfigWindow.cpp ZmdInstallWindow.cpp ZmdServerDialog.cpp xmlrpciface.cpp ZmdUpdaterCore.cpp ZmdProgressDialog.cpp ZmdCatalogListItem.cpp ZmdDependencyDialog.cpp ZmdUpdater.cpp UpdaterApplication.cpp GeneralConfigWindow.cpp UpdateListItem.cpp ZmdAdvancedConfig.cpp ZmdEditServers.cpp ZmdRugParser.cpp

# these are the headers for your project that won't be installed
noinst_HEADERS = MainWindow.h HeaderWidget.h ZmdConfigWindow.h ZmdInstallWindow.h ZmdServerDialog.h xmlrpciface.h ZmdUpdaterCore.h ZmdProgressDialog.h ZmdCatalogListItem.h ZmdDependencyDialog.h Updater.h ZmdUpdater.h UpdaterApplication.h GeneralConfigWindow.h UpdateListItem.h ZmdAdvancedConfig.h ZmdEditServers.h ZmdRugParser.h


# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO
KDE_ICON = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kzmdupdater.pot

# this is where the desktop file will go 
desktopdir   = $(kde_appsdir)/Utilities
desktop_DATA = suseupdater.desktop

install-data-local:
	$(mkinstalldirs) $(DESTDIR)/$(prefix)/share/autostart
	$(INSTALL_DATA) $(srcdir)/suseupdater-start.desktop \
		$(DESTDIR)/$(prefix)/share/autostart/suseupdater.desktop
