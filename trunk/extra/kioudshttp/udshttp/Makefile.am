SUBDIRS= test

INCLUDES    = $(all_includes)

kde_module_LTLIBRARIES = kio_udshttp.la

kio_udshttp_la_SOURCES = udshttp.cpp
kio_udshttp_la_LIBADD = -lkio        
kio_udshttp_la_LDFLAGS = -module -avoid-version $(all_libraries) $(KDE_PLUGIN)

protocol_DATA = udshttp.protocol
protocoldir = $(kde_servicesdir)

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kio_udshttp.pot
