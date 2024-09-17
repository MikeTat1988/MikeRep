# Variables
CXX = g++
CXXFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -g
DEPFLAGS = -MMD -MT"$@"
INCLUDES = -Iinclude

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)
LIB = libassignment.a
OUT = benchmark

DIRS = log data

# Phony targets
.PHONY: $(OUT) clean

# Default target
all: dirs $(OUT)

# Rule to create necessary directories
dirs:
	mkdir -p $(DIRS)

# Rule to make the output file
$(OUT): test/benchmark.cpp $(LIB)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ 

# Rule to make the library
$(LIB): $(OBJ)
	ar rcs $@ $^

# Generic rule for .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEPFLAGS) -c $< -o $@

# Include dependencies
-include $(DEP)

# Clean rule
clean:
	rm -f $(OBJ) $(DEP) $(LIB) ./data/*
