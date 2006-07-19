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
SUSEUpdater_SOURCES = 

# these are the headers for your project that won't be installed
noinst_HEADERS = 


# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

