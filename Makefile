CXX      ?= g++
AR       ?= ar
CXXFLAGS ?= -std=c++11 -O2 -g3 -Werror
CXXFLAGS += -Wall

CXXFLAGS += $(shell sdl-config --cflags)

# Common lib

COMMONSRCS = TextRenderer.cpp DriverFramework.cpp SDLSurface.cpp \
	     Texture.cpp SDL_utils.cpp Color.cpp Math.cpp Clock.cpp \
	     Steering.cpp Random.cpp Matrix22.cpp Matrix44.cpp Quaternion.cpp \
	     Line.cpp Geometry.cpp
COMMONOBJS = $(COMMONSRCS:.cpp=.o)
COMMONDEPS = $(COMMONSRCS:.cpp=.dep)
COMMONLIB = libcommon.a

BINDIR = bin
TESTBIN = common_test
TESTSRCS = GeometryTest.cpp QuadtreeTest.cpp MathTest.cpp test.cpp
TESTOBJS = $(TESTSRCS:.cpp=.o)
TESTDEPS = $(TESTSRCS:.cpp=.dep)


.PHONY: clean all

all: $(COMMONLIB) $(TESTBIN)

$(BINDIR):
	mkdir -p $(BINDIR)

$(TESTBIN): $(BINDIR) $(TESTOBJS) $(COMMONLIB)
	$(CXX) $(CXXFLAGS) $(TESTOBJS) $(COMMONLIB) -o $(BINDIR)/$(TESTBIN)

$(COMMONLIB): $(COMMONOBJS)
	$(AR) rcs $(COMMONLIB) $(COMMONOBJS)

%.dep: %.cpp
	@rm -f $@
	@$(CXX) -MM $(CXXFLAGS) $< > $@.P
	@sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $*)\1.o $@ : ,g' < $@.P > $@
	@rm -f $@.P

clean:
	find . -name '*.o' -exec rm -rf {} +
	find . -name '*.dep' -exec rm -rf {} +
	find . -name '*.a' -exec rm -rf {} +
	rm -rf $(BINDIR)

-include $(COMMONDEPS)
-include $(TESTDEPS)

