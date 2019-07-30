AM_CPPFLAGS = -I$(top_srcdir)/ \
	$(GTEST_CFLAGS) \
	$(GMOCK_CFLAGS)
AM_CXXFLAGS = \
	$(GTEST_MAIN_CFLAGS)
AM_LDFLAGS = \
	$(GMOCK_LIBS) \
	$(GTEST_MAIN_LIBS) \
	$(OESDK_TESTCASE_FLAGS)

# Run all 'check' test programs
check_PROGRAMS = \
	handler_unittest \
	handler_session_unittest
TESTS = $(check_PROGRAMS)

handler_unittest_SOURCES = handler_unittest.cpp
handler_unittest_LDADD = $(top_builddir)/libhandler.la

handler_session_unittest_SOURCES = handler_session_unittest.cpp
handler_session_unittest_LDADD = $(top_builddir)/libhandler.la
