CCC = g++
CFLAGS = -Wall -I/urs/local/include
#LDFLAGS = -lGL -lGLEW -lGLU -lglut  -lm  -llapack -lblas
LDFLAGS = -lGL -lGLU -lglut  -lm

SRCDIR = src
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

DEBUBFLAG = -g

EXEC = $(BINDIR)/main

all: $(EXEC)

$(EXEC): $(OBJECTS)
		$(CCC) -g -o $@ $^ $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
		@mkdir -p $(BINDIR)
		$(CCC) -g -o $@ -c $< $(CFLAGS)


clean:
		rm -rf $(BINDIR)

mrproper: clean
		rm -rf $(EXEC)

