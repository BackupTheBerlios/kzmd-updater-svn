SUBDIRS = doc icons images kxmlrpcclient kzmdauthutil po udshttp updater

$(top_srcdir)/configure.in: configure.in.in $(top_srcdir)/subdirs
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in ;

$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs

$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in > acinclude.m4

$(top_srcdir)/opensuse-updater.spec: opensuse-updater.spec.in
	cd $(top_srcdir) && cp opensuse-updater.spec.in opensuse-updater.spec ;

MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 configure.files opensuse-updater.spec

package-messages:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common package-messages
	$(MAKE) -C po merge

EXTRA_DIST = admin COPYING configure.in.in

dist-hook:
	cd $(top_distdir) && perl admin/am_edit -padmin
	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs

include admin/Doxyfile.am

.PHONY: package package-local check-up-to-date

package: 	package-local
#package: 	check-up-to-date package-local

package-local:	dist-bzip2 opensuse-updater.spec
	@if [ `bunzip2 -c $(distdir).tar.bz2 | wc --bytes` = 0 ] ; then \
		echo "Error: $(distdir).tar.bz2 is empty" ; \
		exit 1 ; \
	fi
	rm -f package/*.tar.bz2
	rm -f package/*~
	rm -f package/*.bak
	rm -f package/*.auto
	mv $(distdir).tar.bz2 package/
	mv opensuse-updater.spec package/

check-up-to-date:
	cd $(srcdir) && ! LC_ALL=C svn status --show-updates --quiet | grep -v '^Status against revision'