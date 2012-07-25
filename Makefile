CXX      ?= g++
AR       ?= ar
CXXFLAGS ?= -std=c++11 -O2 -g3 -Werror
CXXFLAGS += -Wall

CXXFLAGS += $(shell sdl-config --cflags)

CXXFLAGS += -Isrc

# Common lib

COMMONSRCFILES = SDLSurface.cpp Texture.cpp SDL_utils.cpp Color.cpp Math.cpp
COMMONSRCDIR = src
COMMONSRCS = $(addprefix $(COMMONSRCDIR)/, $(COMMONSRCFILES))
COMMONOBJS = $(COMMONSRCS:.cpp=.o)
COMMONDEPS = $(COMMONSRCS:.cpp=.dep)
COMMONLIB = $(COMMONSRCDIR)/libcommon.a


.PHONY: clean all

all: $(COMMONLIB)

$(COMMONLIB): $(COMMONOBJS)
	$(AR) rcs $(COMMONLIB) $(COMMONOBJS)

%.dep: %.cpp
	@rm -f $@
	@$(CC) -MM $(CXXFLAGS) $< > $@.P
	@sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $*)\1.o $@ : ,g' < $@.P > $@
	@rm -f $@.P

clean:
	find src/ -name '*.o' -exec rm -rf {} +
	find src/ -name '*.dep' -exec rm -rf {} +
	find src/ -name '*.a' -exec rm -rf {} +

-include $(COMMONDEPS)

