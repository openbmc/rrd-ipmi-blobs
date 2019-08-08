AM_CPPFLAGS = -I$(top_srcdir)/ \
	$(GTEST_CFLAGS) \
	$(GMOCK_CFLAGS)
AM_CXXFLAGS = \
	$(GTEST_MAIN_CFLAGS)
AM_LDFLAGS = \
	$(GMOCK_LIBS) \
	$(GTEST_MAIN_LIBS) \
	$(OESDK_TESTCASE_FLAGS)

check_PROGRAMS = \
	handler_unittest \
	service_unittest

handler_unittest_SOURCES = handler_unittest.cpp \
			   handler_session_unittest.cpp \
			   handler_buffer_unittest.cpp
handler_unittest_LDADD = $(top_builddir)/libhandler.la

service_unittest_SOURCES = service_unittest.cpp \
			   service_last_unittest.cpp
service_unittest_LDADD = $(top_builddir)/libservice.la

TESTS = $(check_PROGRAMS)
