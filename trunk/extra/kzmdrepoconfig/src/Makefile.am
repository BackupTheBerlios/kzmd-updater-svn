# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kzmdrepoconfig
# set the include path for X, qt and KDE
INCLUDES = $(all_includes) -I$(top_srcdir)/kxmlrpcclient 

# the library search path. 
kzmdrepoconfig_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
kzmdrepoconfig_LDADD   = $(LIB_KFILE) $(top_srcdir)/kxmlrpcclient/libkxmlrpcclient.la -lkxmlrpcclient -lrss

# which sources should be compiled for kzmdupdater
kzmdrepoconfig_SOURCES = main.cpp ZmdUpdaterCore.cpp MainWindow.cpp RepoFeed.cpp HeaderWidget.cpp FeedDialog.cpp
# these are the headers for your project that won't be installed
noinst_HEADERS = ZmdUpdaterCore.h ZmdUpdaterCoreUtils.h MainWindow.h RepoFeed.h HeaderWidget.h FeedDialog.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO
