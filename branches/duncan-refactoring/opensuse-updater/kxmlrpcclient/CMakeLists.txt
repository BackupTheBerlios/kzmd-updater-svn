INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR}  )


########### next target ###############

SET(kxmlrpcclient_LIB_SRCS
kxmlrpcserver.cpp
kxmlrpcquery.cpp
)

KDE3_AUTOMOC(${kxmlrpcclient_LIB_SRCS})
ADD_LIBRARY(kxmlrpcclient SHARED ${kxmlrpcclient_LIB_SRCS})
TARGET_LINK_LIBRARIES(kxmlrpcclient  ${QT_AND_KDECORE_LIBS} )
SET_TARGET_PROPERTIES(kxmlrpcclient PROPERTIES VERSION 4.2.0 SOVERSION 4 )
INSTALL(TARGETS kxmlrpcclient DESTINATION ${KDE3_LIB_DIR} )


########### install files ###############

INSTALL_FILES( /include FILES  kxmlrpcserver.h )



#original Makefile.am contents follow:

#lib_LTLIBRARIES = libkxmlrpcclient.la
#
## set the include path for X, qt and KDE
#INCLUDES = $(all_includes)
#
#messages: rc.cpp
#	$(EXTRACTRC) *.ui >> rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kxmlrpcclient.pot
#
## the library search path. 
#libkxmlrpcclient_la_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#
## the libraries to link against.
#libkxmlrpcclient_la_LIBADD   = $(LIB_KFILE)
#
## which sources should be compiled for kzmdupdater
#libkxmlrpcclient_la_SOURCES = kxmlrpcserver.cpp kxmlrpcquery.cpp
#
#include_HEADERS = kxmlrpcserver.h
#
#noinst_HEADERS = kxmlrpcquery.h
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
#include $(top_srcdir)/admin/Doxyfile.am
#
