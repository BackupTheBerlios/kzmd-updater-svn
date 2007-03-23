INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/kxmlrpcclient ${CMAKE_SOURCE_DIR}/updater ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR}  )


########### next target ###############

SET(opensuseupdater_zmd_LIB_SRCS
ZmdAdvancedConfig.cpp
ZmdEditServers.cpp
ZmdRugParser.cpp
ZmdConfigWindow.cpp
ZmdInstallWindow.cpp
ZmdServerDialog.cpp
ZmdUpdaterCore.cpp
ZmdProgressDialog.cpp
ZmdCatalogListItem.cpp
ZmdDependencyDialog.cpp
ZmdUpdater.cpp
ZmdUpdateListItem.cpp
)

KDE3_AUTOMOC(${opensuseupdater_zmd_LIB_SRCS})
KDE3_ADD_KPART(opensuseupdater_zmd ${opensuseupdater_zmd_LIB_SRCS})
TARGET_LINK_LIBRARIES(opensuseupdater_zmd  ${QT_AND_KDECORE_LIBS} kxmlrpcclient opensuseupdater )
SET_TARGET_PROPERTIES(opensuseupdater_zmd PROPERTIES VERSION 4.2.0 SOVERSION 4 )
INSTALL(TARGETS opensuseupdater_zmd DESTINATION ${KDE3_PLUGIN_INSTALL_DIR} )

#original Makefile.am contents follow:

#lib_LTLIBRARIES = libopensuseupdater_zmd.la
#
## set the include path for X, qt and KDE
#INCLUDES = $(all_includes) -I$(top_srcdir)/kxmlrpcclient -I$(top_srcdir)/updater
#
## the library search path. 
#libopensuseupdater_zmd_la_LDFLAGS = $(KDE_RPATH) $(all_libraries) 
#
## the libraries to link against.
#libopensuseupdater_zmd_la_LIBADD   = $(LIB_KFILE) $(top_builddir)/kxmlrpcclient/libkxmlrpcclient.la
#opensuseupdater_zmd_la_LDADD = $(top_builddir)/updater/libopensuseupdater.la
#
## which sources should be compiled for kzmdupdater
#libopensuseupdater_zmd_la_SOURCES = ZmdAdvancedConfig.cpp ZmdEditServers.cpp ZmdRugParser.cpp ZmdConfigWindow.cpp ZmdInstallWindow.cpp ZmdServerDialog.cpp ZmdUpdaterCore.cpp ZmdProgressDialog.cpp ZmdCatalogListItem.cpp ZmdDependencyDialog.cpp ZmdUpdater.cpp ZmdUpdateListItem.cpp
#
#noinst_HEADERS = ZmdAdvancedConfig.h ZmdEditServers.h ZmdRugParser.h ZmdUpdater.h ZmdConfigWindow.h ZmdInstallWindow.h ZmdServerDialog.h ZmdUpdaterCore.h ZmdProgressDialog.h ZmdCatalogListItem.h ZmdDependencyDialog.h
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
#messages: rc.cpp
#	$(EXTRACTRC) *.ui >> rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/opensuseupdater.pot
#
#include $(top_srcdir)/admin/Doxyfile.am
#
