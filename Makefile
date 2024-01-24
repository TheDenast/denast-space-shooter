# Makefile for the project

# Compiler to use
CXX = g++
# Compiler flags
CXXFLAGS = -Wall -c
# Linker flags
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Directory for source files
SRC_DIR = src
# Directory for object files
OBJ_DIR = obj
# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
# Executable name
EXECUTABLE = dss-game

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)
