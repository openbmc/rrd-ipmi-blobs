AM_DEFAULT_SOURCE_EXT = .cpp

librrdblobdir = ${libdir}/ipmid-providers
librrdblob_LTLIBRARIES = librrdblob.la
librrdblob_la_SOURCES = main.cpp
librrdblob_la_LIBADD = libhandler.la
librrdblob_la_LDFLAGS = -version-info 0:0:0 -shared
librrdblob_la_CXXFLAGS = -flto

noinst_LTLIBRARIES = libhandler.la
libhandler_la_SOURCES = handler.cpp
libhandler_la_LDFLAGS = $(PHOSPHOR_LOGGING_LIBS)
libhandler_la_CXXFLAGS = $(PHOSPHOR_LOGGING_CFLAGS)

SUBDIRS = . test
