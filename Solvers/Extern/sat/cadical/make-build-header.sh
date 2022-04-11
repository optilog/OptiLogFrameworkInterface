#!/bin/sh

#--------------------------------------------------------------------------#

# Used to generate 'build.hpp', which in turn is used in 'version.cpp' to
# print compile time options, compiler version, and source code version.
# If generating 'build.hpp' fails you can still compile 'version.cpp' by
# using the '-DNBUILD' compile time option.

#--------------------------------------------------------------------------#

die () {
  echo "make-build-header.sh: error: $*" 1>&2
  exit 1
}

warning () {
  echo "make-build-header.sh: warning: $*" 1>&2
}

#--------------------------------------------------------------------------#
# The version.
#
VERSION="`cat VERSION`"
if [ x"$VERSION" = x ]
then
  warning "could not determine 'VERSION'"
else
  echo "#define VERSION \"$VERSION\""
fi

#--------------------------------------------------------------------------#
# The unique GIT hash.
#
IDENTIFIER="`./get-git-id.sh`"
if [ x"$IDENTIFIER" = x ]
then
  warning "could not determine 'IDENTIFIER' (git id)"
else
  echo "#define IDENTIFIER \"$IDENTIFIER\""
fi

#--------------------------------------------------------------------------#
# C++ compiler 'CXX' used in 'makefile'.
#
if [[ "$OSTYPE" == "darwin"* ]]; then
  SEDMAC=-E
else
  SEDMAC=
fi

  COMPILER="`sed $SEDMAC -e '/^CXX[ \t]+:=/!d' -e 's,^CXX[ \t]+:= ,,' Makefile 2>/dev/null`"
case x"$COMPILER" in
  xc++* | xg++* | xclang++*)
    COMPILER="`$COMPILER --version 2>/dev/null|head -1`";;
  *) COMPILER="";;
esac
if [ x"$COMPILER" = x ]
then
  warning "could not determine 'COMPILER' ('CXX')"
else
  echo "#define COMPILER \"$COMPILER\""
fi

#--------------------------------------------------------------------------#
# C++ compiler flags 'CXXFLAGS' used in 'makefile'.
#
FLAGS="`sed $SEDMAC -e '/^CXXFLAGS[ \t]+:=/!d' -e 's,^CXXFLAGS[ \t]+:= ,,' Makefile 2>/dev/null`"
if [ x"$FLAGS" = x ]
then
  warning "could not determine 'FLAGS' ('CXXFLAGS')"
else
  echo "#define FLAGS \"$FLAGS\""
fi

#--------------------------------------------------------------------------#
# Use time of executing this script as build time.
#
LC_TIME="en_US" # Avoid umlaut in 'DATE'.
export LC_TIME
# The time and date we compiled the CaDiCaL library.
DATE="`date 2>/dev/null|sed -e 's,  *, ,g'`"
OS="`uname -srmn 2>/dev/null`"
DATE="`echo $DATE $OS|sed -e 's,^ *,,' -e 's, *$,,'`"
if [ x"$DATE" = x" " ]
then
  warning "could not determine 'DATE' (build date and time)"
else
  echo "#define DATE \"$DATE\""
fi
