### Makefile ---

## Author: aign
## Keywords:

# main parameters
CXX      := c++
CXXFLAGS := -std=c++11 -fPIC -Wall -Wno-deprecated -fno-strict-aliasing -DQUIET
CXXOPT   := -O3 -DNDEBUG
CXXDEBUG := -O0 -g3
CXXPROF  := -O3 -g3 -fno-inline -fno-omit-frame-pointer -pg -DNDEBUG
INCLUDES := -I.
LIBS     := -L.
SOURCES  := analyze.cpp arena.cpp assume.cpp averages.cpp backtrack.cpp backward.cpp bins.cpp block.cpp ccadical.cpp checker.cpp clause.cpp collect.cpp compact.cpp config.cpp cover.cpp decide.cpp decompose.cpp deduplicate.cpp elim.cpp ema.cpp extend.cpp external.cpp file.cpp flags.cpp format.cpp gates.cpp instantiate.cpp internal.cpp ipasir.cpp limit.cpp logging.cpp lucky.cpp message.cpp minimize.cpp occs.cpp options.cpp parse.cpp phases.cpp probe.cpp profile.cpp proof.cpp propagate.cpp queue.cpp random.cpp reduce.cpp rephase.cpp report.cpp resources.cpp restart.cpp restore.cpp score.cpp signal.cpp solution.cpp solver.cpp stats.cpp subsume.cpp terminal.cpp ternary.cpp tracer.cpp transred.cpp util.cpp var.cpp version.cpp vivify.cpp walk.cpp watch.cpp
OBJECTS  := $(SOURCES:.cpp=.o)
TRGT     := cadical

# gather compiler options
CXXARGS := $(CXXFLAGS) $(INCLUDES) $(CXXOPT)

# targets
.PHONY: all clean lib

all: lib

lib: build.hpp $(OBJECTS)
	@-ar cr lib$(TRGT).a $(OBJECTS)

build.hpp:
	./make-build-header.sh > build.hpp

%.o: %.cpp
	$(CXX) $(CXXARGS) -c $< -o $@

clean:
	@-rm $(OBJECTS)

allclean:
	@-find . -name '*.o' -delete
	@-find . -name '*.a' -delete

#-------------------------------------------------------------------------------
