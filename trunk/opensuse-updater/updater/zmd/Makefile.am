lib_LTLIBRARIES = libzmdqt.la

# set the include path for X, qt and KDE
INCLUDES = $(all_includes) -I$(top_srcdir)/kxmlrpcclient -I$(top_srcdir)/updater

# the library search path. 
libzmdqt_la_LDFLAGS = $(KDE_RPATH) $(all_libraries) 

# the libraries to link against.
libzmdqt_la_LIBADD   = $(LIB_KFILE) $(top_builddir)/kxmlrpcclient/libkxmlrpcclient.la -lkxmlrpcclient

# which sources should be compiled for kzmdupdater
libzmdqt_la_SOURCES = ZmdAdvancedConfig.cpp ZmdEditServers.cpp ZmdRugParser.cpp ZmdConfigWindow.cpp ZmdInstallWindow.cpp ZmdServerDialog.cpp ZmdUpdaterCore.cpp ZmdProgressDialog.cpp ZmdCatalogListItem.cpp ZmdDependencyDialog.cpp ZmdUpdater.cpp

noinst_HEADERS = ZmdAdvancedConfig.h ZmdEditServers.h ZmdRugParser.h ZmdUpdater.h ZmdConfigWindow.h ZmdInstallWindow.h ZmdServerDialog.h ZmdUpdaterCore.h ZmdProgressDialog.h ZmdCatalogListItem.h ZmdDependencyDialog.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

include $(top_srcdir)/admin/Doxyfile.am

