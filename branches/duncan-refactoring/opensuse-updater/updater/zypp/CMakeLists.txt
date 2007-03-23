INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/updater ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR}  )


########### next target ###############

SET(opensuseupdater_zypp_PART_SRCS
ZYppUpdater.cpp
ZYppListItem.cpp
)

KDE3_AUTOMOC(${opensuseupdater_zypp_PART_SRCS})
KDE3_ADD_KPART(opensuseupdater_zypp ${opensuseupdater_zypp_PART_SRCS})
TARGET_LINK_LIBRARIES(opensuseupdater_zypp  ${QT_AND_KDECORE_LIBS} opensuseupdater )
INSTALL_TARGETS(/lib/kde3 opensuseupdater_zypp )

########### install files ###############

INSTALL_FILES( /share/services FILES  opensuseupdater_zypp.desktop )

