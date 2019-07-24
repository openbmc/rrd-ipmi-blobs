AM_DEFAULT_SOURCE_EXT = .cpp

librrdblobdir = ${libdir}/ipmid-providers
librrdblob_LTLIBRARIES = librrdblob.la
librrdblob_la_SOURCES = main.cpp \
			handler.cpp
librrdblob_la_LDFLAGS = $(PHOSPHOR_LOGGING_LIBS) \
			-version-info 0:0:0 -shared
librrdblob_la_CXXFLAGS = $(PHOSPHOR_LOGGING_CFLAGS) \
			-flto

SUBDIRS = . test
