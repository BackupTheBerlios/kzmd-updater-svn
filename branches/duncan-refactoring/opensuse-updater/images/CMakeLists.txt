INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR}  )


########### install files ###############

INSTALL_FILES( /share/apps/opensuseupdater/pics FILES  suse_logo.png opensuseupdater.png )



#original Makefile.am contents follow:

#images_data_DATA=suse_logo.png opensuseupdater.png
#images_datadir= $(kde_datadir)/opensuseupdater/pics
