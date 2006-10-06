lib_LTLIBRARIES = libopensuseupdater_zypp

# set the include path for X, qt and KDE
INCLUDES = $(all_includes) -I$(top_srcdir)/kxmlrpcclient -I$(top_srcdir)/updater

# the library search path. 
libopensuseupdater_zypp_la_LDFLAGS = $(KDE_RPATH) $(all_libraries) 

# the libraries to link against.
libopensuseupdater_zypp_la_LIBADD   = $(LIB_KFILE) $(top_builddir)/kxmlrpcclient/libkxmlrpcclient.la -lkxmlrpcclient

# which sources should be compiled for kzmdupdater
libopensuseupdater_zypp_la_SOURCES = ZYppUpdater.cpp

noinst_HEADERS = ZYppUpdater.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

include $(top_srcdir)/admin/Doxyfile.am

