# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
#SUBDIRS =

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kzmdupdater

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# the library search path. 
kzmdupdater_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
kzmdupdater_LDADD   = $(LIB_KFILE)

# which sources should be compiled for kzmdupdater
kzmdupdater_SOURCES = main.cpp  SUSEUpdater.cpp HeaderWidget.cpp

# these are the headers for your project that won't be installed
noinst_HEADERS = SUSEUpdater.h HeaderWidget.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kzmdupdater.pot

# this is where the desktop file will go 
desktopdir   = $(kde_appsdir)/Utilities
desktop_DATA = kzmdupdater.desktop
