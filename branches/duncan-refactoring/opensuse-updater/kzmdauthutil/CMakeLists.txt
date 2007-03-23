INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR}  )


########### next target ###############

SET(kzmdauthutil_SRCS
kzmdauthutil.c
)

KDE3_AUTOMOC(${kzmdauthutil_SRCS})

KDE3_ADD_EXECUTABLE(kzmdauthutil ${kzmdauthutil_SRCS})

TARGET_LINK_LIBRARIES(kzmdauthutil  ${QT_AND_KDECORE_LIBS} )

INSTALL_TARGETS(/bin kzmdauthutil )


########### install files ###############




#original Makefile.am contents follow:

## this has all of the subdirectories that make will recurse into.  if
## there are none, comment this out
##SUBDIRS =
#
## this is the program that gets installed.  it's name is used for all
## of the other Makefile.am variables
#bin_PROGRAMS = kzmdauthutil
#
#kzmdauthutil_SOURCES = kzmdauthutil.c
#
