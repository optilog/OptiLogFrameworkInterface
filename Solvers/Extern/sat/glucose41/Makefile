### Makefile ---

## Author: aign
## Keywords:

# main parameters
CXX      := c++
CXXFLAGS := -std=c++11 -fPIC -Wall -Wno-deprecated -fno-strict-aliasing -DINCREMENTAL
CXXOPT   := -O3 -DNDEBUG
CXXDEBUG := -O0 -g3
CXXPROF  := -O3 -g3 -fno-inline -fno-omit-frame-pointer -pg -DNDEBUG
INCLUDES := -I..
LIBS     := -L.
SOURCES  := core/Solver.cc utils/Options.cc utils/System.cc
OBJECTS  := $(SOURCES:.cc=.o)
TRGT     :=  glucose41

# gather compiler options
CXXARGS := $(CXXFLAGS) $(INCLUDES) $(CXXOPT)

# targets
.PHONY: all clean lib

all: lib

lib: $(OBJECTS)
	@-ar cr lib$(TRGT).a $(OBJECTS)

%.o: %.cc
	$(CXX) $(CXXARGS) -c $< -o $@

clean:
	@-rm $(OBJECTS)

allclean:
	@-find . -name '*.o' -delete
	@-find . -name '*.a' -delete

#-------------------------------------------------------------------------------
