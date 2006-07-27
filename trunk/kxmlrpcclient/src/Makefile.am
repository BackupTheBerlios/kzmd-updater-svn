lib_LTLIBRARIES = libkxmlrpcclient.la

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# the library search path. 
libkxmlrpcclient_la_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
libkxmlrpcclient_la_LIBADD   = $(LIB_KFILE)

# which sources should be compiled for kzmdupdater
libkxmlrpcclient_la_SOURCES = kxmlrpcclient.cpp kxmlrpcclient.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

