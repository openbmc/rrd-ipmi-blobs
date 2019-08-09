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
	handler_unittest

handler_unittest_SOURCES = handler_unittest.cpp \
			   handler_session_unittest.cpp \
			   handler_buffer_unittest.cpp
handler_unittest_LDADD = $(top_builddir)/libhandler.la


TESTS = $(check_PROGRAMS)
