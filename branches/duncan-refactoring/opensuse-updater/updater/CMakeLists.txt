#ADD_SUBDIRECTORY( zmd ) 
#ADD_SUBDIRECTORY( zypp ) 

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/kxmlrpcclient ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR}  )

########### next target ###############

kde3_add_kcfg_files(settings_SRCS opensuseupdater_settings.kcfgc)
SET(opensuseupdater_LIB_SRCS
Updater.cpp osupdaterpopup.cpp TrayIcon.cpp UpdateListItem.cpp HeaderWidget.cpp ${settings_SRCS}
)
kde3_add_ui_files( opensuseupdater_LIB_SRCS updatersettingswidget_base.ui )
KDE3_AUTOMOC(${opensuseupdater_LIB_SRCS})
ADD_LIBRARY(opensuseupdater SHARED ${opensuseupdater_LIB_SRCS})
SET_TARGET_PROPERTIES(opensuseupdater PROPERTIES VERSION 4.2.0 SOVERSION 4 )
TARGET_LINK_LIBRARIES(opensuseupdater  ${QT_AND_KDECORE_LIBS} kdeui )
INSTALL(TARGETS opensuseupdater DESTINATION ${KDE3_LIB_INSTALL_DIR} )


########### next target ###############

SET(opensuseupdater_SRCS main.cpp UpdaterApplication.cpp  MainWindow.cpp  )
KDE3_AUTOMOC(${opensuseupdater_SRCS})
KDE3_ADD_EXECUTABLE(opensuseupdater-kde ${opensuseupdater_SRCS})
TARGET_LINK_LIBRARIES(opensuseupdater-kde opensuseupdater ${QT_AND_KDECORE_LIBS} kdeui kio )
INSTALL_TARGETS(/bin opensuseupdater-kde )


########### install files ###############

INSTALL_FILES( /share/config.kcfg FILES  opensuseupdater_settings.kcfg )
INSTALL_FILES( /share/servicetypes FILES  opensuseupdater_backend.desktop )
INSTALL_FILES(  FILES  opensuseupdater-su )
INSTALL_FILES( /share/applnk/Utilities FILES  opensuseupdater.desktop )

KDE3_INSTALL_ICONS( hicolor )
