# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kzmduserconfig 

# set the include path for X, qt and KDE
INCLUDES = $(all_includes) $(top_srcdir)/kxmlrpcclient 

# the library search path. 
kzmduserconfig_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
kzmduserconfig_LDADD   = $(LIB_KFILE) $(top_srcdir)/kxmlrpcclient/libkxmlrpcclient.la -lkxmlrpcclient

# which sources should be compiled for kzmdupdater
kzmduserconfig_SOURCES = main.cpp ZmdUpdaterCore.cpp MainWindow.cpp EditWindow.cpp
# these are the headers for your project that won't be installed
noinst_HEADERS = ZmdUpdaterCore.h ZmdUpdaterCoreUtils.h MainWindow.h EditWindow.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO
