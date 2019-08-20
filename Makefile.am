AM_DEFAULT_SOURCE_EXT = .cpp

ipmiddir = ${libdir}/ipmid-providers
ipmid_LTLIBRARIES = librrdblob.la

librrdblob_la_SOURCES = main.cpp
librrdblob_la_LIBADD = libhandler.la libservice.la librrdplus.la
librrdblob_la_LDFLAGS = -version-info 0:0:0 -shared
librrdblob_la_CXXFLAGS = -flto


noinst_LTLIBRARIES = libhandler.la libservice.la

libhandler_la_SOURCES = handler.cpp
libhandler_la_CPPFLAGS = $(PHOSPHOR_LOGGING_CFLAGS)
libhandler_la_LIBADD = $(PHOSPHOR_LOGGING_LIBS)

libservice_la_SOURCES = service.cpp rrd.pb.cc
libservice_la_CPPFLAGS = $(PROTOBUF_CFLAGS)
libservice_la_LIBADD = $(PROTOBUF_LIBS)


dist_noinst_DATA = rrd.proto
BUILT_SOURCES = rrd.pb.h rrd.pb.cc

%.pb.cc %.pb.h: %.proto
	$(AM_V_GEN)$(PROTOC) --proto_path=$(srcdir) --cpp_out=$(builddir) $^


lib_LTLIBRARIES = librrdplus.la

librrdplus_la_SOURCES = rrd.cpp
librrdplus_la_CPPFLAGS = $(LIBRRD_CFLAGS)
librrdplus_la_LIBADD = $(LIBRRD_LIBS)


SUBDIRS = . test
