#!/bin/bash
#
# Folder structure
# project/ 			-	Project folder
# project/src			- 	All .cpp files (sub folders also included)
# project/inc			-	All .h files
#
# make clean - to delete all output files
# make build - to build only
# make all   - to compile and link
# make run   - to compile, link and run
#

# This is the main compiler
CC := g++

# Source file directory
SRCDIR := src

# Include directories
# Don't remove -I
INC := -I inc

# Build directory - to store object files
BUILDDIR := build

# Output directory
OUTPUT = bin

# Target or Executible file
TARGET := $(OUTPUT)/runner

# Source file extension
SRCEXT := cpp

# Compiler flags
CFLAGS :=  #-g -g3 -O2 -ggdb -m64 -Wall #-D debug=1

# Include libraries
LIB :=

# Creating list of cpp files
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")

# Creating list of files to be built
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Default task
all: build link

$(TARGET): $(OBJECTS)
	@echo "\n# Linking"
	@mkdir -p $(OUTPUT)
	$(CC) $^ -o $(TARGET) $(LIB)
	@echo "\n# Executable Binary: $(TARGET)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@if [ ! -f $@ ]; then mkdir -p $@; rm -r $@; fi
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<


# Cleaning all files
clean:
	@echo "# Cleaning";
	@$(RM) -r -v $(BUILDDIR) $(OUTPUT)
	@echo ""

# Building
build: .print $(OBJECTS)
	@echo ""

# Linking
link: $(TARGET)

# Build and run
run: all
	@echo "# Running"
	@./$(TARGET)
	@echo ""

.PHONY: clean

.print:
	@echo "# Building"

help:
	@echo 'make <all, clean, build, link, run>'
