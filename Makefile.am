AM_DEFAULT_SOURCE_EXT = .cpp

librrdblobdir = ${libdir}/ipmid-providers
librrdblob_LTLIBRARIES = librrdblob.la
librrdblob_la_SOURCES = main.cpp \
			handler.cpp
librrdblob_la_LDFLAGS = -version-info 0:0:0 -shared
librrdblob_la_CXXFLAGS = -flto

SUBDIRS = . test
