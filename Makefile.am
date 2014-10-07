AM_LDFLAGS=-avoid-version -module -export-dynamic
ACLOCAL_AMFLAGS=-I m4

lib_LTLIBRARIES = libresolvconf_override.la

libresolvconf_override_la_SOURCES = resolvconf-override.c
libresolvconf_override_la_CFLAGS=$(AM_CFLAGS)
libresolvconf_override_la_LDFLAGS=$(AM_LDFLAGS)

check_PROGRAMS = test
TESTS = $(check_PROGRAMS)

test_SOURCES = test.c

AM_TESTS_ENVIRONMENT = LD_PRELOAD=$(DESTDIR)$(libdir)/libresolvconf_override.so

install-exec-hook:
	rm -rf $(DESTDIR)$(libdir)/libresolvconf_override.la

uninstall-hook:
	rm -rf $(DESTDIR)$(libdir)/libresolvconf_override.la
