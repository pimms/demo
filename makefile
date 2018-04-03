LIBS=
CXX := gcc
CFLAGS = -Os -std=c99

SOURCEDIR := src
OBJECTDIR := obj
BUILDDIR := build


UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	CFLAGS += -I/System/Library/Frameworks/OpenGL.framework/Headers/
	CFLAGS += -I/System/Library/Frameworks/Cocoa.framework/Headers/
	CFLAGS += -I/System/Library/Frameworks/GLUT.framework/Headers/
	CFLAGS += -framework OpenGL
	CFLAGS += -framework GLUT
	CLFAGS += -Wdeprecated-declarations -Wunused-command-line-argument
endif


# Get all .c files
SRC := $(shell find $(SOURCEDIR) -name '*.c')

# Get all the .o files
OBJECTS := $(addprefix $(OBJECTDIR)/,$(notdir $(SRC:%.c=%.o)))

# Name of application
NAME = demo

all: $(NAME)

run: all
	./$(NAME)

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	$(CXX) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	rm -f $(NAME) $(OBJECTS)
