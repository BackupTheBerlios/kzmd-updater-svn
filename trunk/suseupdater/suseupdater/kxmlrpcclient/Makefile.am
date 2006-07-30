lib_LTLIBRARIES = libkxmlrpcserver.la

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# the library search path. 
libkxmlrpcserver_la_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
libkxmlrpcserver_la_LIBADD   = $(LIB_KFILE)

# which sources should be compiled for kzmdupdater
libkxmlrpcserver_la_SOURCES = kxmlrpcserver.cpp kxmlrpcserver.h kxmlrpcquery.h kxmlrpcquery.cpp

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

