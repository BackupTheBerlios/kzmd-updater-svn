lib_LTLIBRARIES = libopensuseupdater_zmd.la

# set the include path for X, qt and KDE
INCLUDES = $(all_includes) -I$(top_srcdir)/kxmlrpcclient -I$(top_srcdir)/updater

# the library search path. 
libopensuseupdater_zmd_la_LDFLAGS = $(KDE_RPATH) $(all_libraries) 

# the libraries to link against.
libopensuseupdater_zmd_la_LIBADD   = $(LIB_KFILE) $(top_builddir)/kxmlrpcclient/libkxmlrpcclient.la

# which sources should be compiled for kzmdupdater
libopensuseupdater_zmd_la_SOURCES = ZmdAdvancedConfig.cpp ZmdEditServers.cpp ZmdRugParser.cpp ZmdConfigWindow.cpp ZmdInstallWindow.cpp ZmdServerDialog.cpp ZmdUpdaterCore.cpp ZmdProgressDialog.cpp ZmdCatalogListItem.cpp ZmdDependencyDialog.cpp ZmdUpdater.cpp ZmdUpdateListItem.cpp

noinst_HEADERS = ZmdAdvancedConfig.h ZmdEditServers.h ZmdRugParser.h ZmdUpdater.h ZmdConfigWindow.h ZmdInstallWindow.h ZmdServerDialog.h ZmdUpdaterCore.h ZmdProgressDialog.h ZmdCatalogListItem.h ZmdDependencyDialog.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) *.ui >> rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/opensuseupdater.pot

include $(top_srcdir)/admin/Doxyfile.am

