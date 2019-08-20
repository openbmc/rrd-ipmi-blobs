AC_PREREQ([2.69])
AC_INIT([rrd-ipmi-blobs], [1.0])
AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AX_WITH_PROG([PROTOC], [protoc], [protoc])

# Checks for packages.
PKG_CHECK_MODULES([LIBRRD], [librrd])
PKG_CHECK_MODULES([PHOSPHOR_LOGGING], [phosphor-logging])
PKG_CHECK_MODULES([PROTOBUF], [protobuf >= 3.0])

PKG_CHECK_MODULES([GMOCK], [gmock], [], [AC_MSG_WARN([gmock package not found.])])
PKG_CHECK_MODULES([GTEST_MAIN], [gtest_main], [], [AC_MSG_WARN([gtest_main package not found.])])

# Checks for header files.
AC_CHECK_HEADER([blobs-ipmid/blobs.hpp], [], [AC_MSG_ERROR([phosphor-ipmi-blobs headers not found.])])

# Checks for compiler characteristics.
AX_CXX_COMPILE_STDCXX_17([noext])
AX_APPEND_COMPILE_FLAGS([-Wall -Werror], [CXXFLAGS])

# Initialize automake.
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])
AM_SILENT_RULES([yes])

# Initialize libtool.
AM_PROG_AR
LT_INIT

# Setup package options.
AC_ARG_VAR([OECORE_TARGET_SYSROOT],
    [Path to the OE SDK target sysroot])
AC_ARG_ENABLE([oe-sdk],
    AS_HELP_STRING([--enable-oe-sdk],
        [Link testcases absolutely against OE SDK so they can be ran within it.]),
    AS_IF([test "x$OECORE_TARGET_SYSROOT" == "x"],
        AC_MSG_ERROR([OECORE_TARGET_SYSROOT must be set with --enable-oe-sdk]),
        [
            AC_MSG_NOTICE([Enabling OE-SDK at $OECORE_TARGET_SYSROOT])
            [
                testcase_flags="-Wl,-rpath,\${OECORE_TARGET_SYSROOT}/lib"
                testcase_flags="${testcase_flags} -Wl,-rpath,\${OECORE_TARGET_SYSROOT}/usr/lib"
                testcase_flags="${testcase_flags} -Wl,-dynamic-linker,`find \${OECORE_TARGET_SYSROOT}/lib/ld-*.so | sort -r -n | head -n1`"
            ]
            AC_SUBST([OESDK_TESTCASE_FLAGS], [$testcase_flags])
        ]
    )
)

# Create configured output.
AC_CONFIG_FILES([Makefile test/Makefile])
AC_OUTPUT
