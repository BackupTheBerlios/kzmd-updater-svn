# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
#SUBDIRS =

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = udstest

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# the library search path. 
udstest_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
udstest_LDADD   = $(LIB_KFILE) $(LIB_KIO)

# which sources should be compiled for kzmdupdater
udstest_SOURCES = udstest.cpp main.cpp
# these are the headers for your project that won't be installed
noinst_HEADERS =  udstest.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

