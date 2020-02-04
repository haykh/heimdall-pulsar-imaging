TARGET   = heimdall

CPP      = g++
CPPFLAGS = --std=c++17

SRCDIR   = src
HDIR     = headers
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(HDIR)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: dirs $(BINDIR)/$(TARGET)

dirs:
	mkdir -p $(BINDIR) $(OBJDIR)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CPP) $(OBJECTS) $(CPPFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BINDIR) $(OBJDIR)
