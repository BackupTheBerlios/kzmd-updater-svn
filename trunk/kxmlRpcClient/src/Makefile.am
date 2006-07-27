lib_LTLIBRARIES = libkxmlrpcclient.la

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# the library search path. 
libxmlrpcclient_la_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
libxmlrpcclient_la_LDADD   = $(LIB_KFILE)

# which sources should be compiled for kzmdupdater
libxmlrpcclient_la_SOURCES = kxmlrpcclient.cpp kxmlrpcclient.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

