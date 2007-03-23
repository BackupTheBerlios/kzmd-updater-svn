ADD_SUBDIRECTORY( test ) 

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR}  )


########### next target ###############

SET(kio_udshttp_PART_SRCS
udshttp.cpp
)

KDE3_AUTOMOC(${kio_udshttp_PART_SRCS})

KDE3_ADD_KPART(kio_udshttp ${kio_udshttp_PART_SRCS})

TARGET_LINK_LIBRARIES(kio_udshttp  ${QT_AND_KDECORE_LIBS} kio )

INSTALL_TARGETS(/lib/kde3 kio_udshttp )


########### install files ###############

INSTALL_FILES( /share/services FILES  udshttp.protocol )



#original Makefile.am contents follow:

#SUBDIRS= test
#
#INCLUDES    = $(all_includes)
#
#kde_module_LTLIBRARIES = kio_udshttp.la
#
#kio_udshttp_la_SOURCES = udshttp.cpp
#kio_udshttp_la_LIBADD = -lkio        
#kio_udshttp_la_LDFLAGS = -module -avoid-version $(all_libraries) $(KDE_PLUGIN)
#
#protocol_DATA = udshttp.protocol
#protocoldir = $(kde_servicesdir)
#
#messages: rc.cpp
#	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kio_udshttp.pot
#
#include $(top_srcdir)/admin/Doxyfile.am
