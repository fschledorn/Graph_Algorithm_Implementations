# Compiler and flags
CXX = g++ # Or clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -g -O3 # Using C++23 and O3 optimization
LDFLAGS = # Add linking flags if needed

# Directories
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
EXT_DIR = external
# Ensure include paths are correct
INCLUDE_DIRS = -I$(CORE_DIR) -I$(EXT_DIR)/nlohmann_json/single_include -I$(SRC_DIR)

# Source files (Only list non-template implementation files)
# Assuming you have a main.cc file in src/
# --- Make sure this line reflects your actual source files ---
SOURCES = $(SRC_DIR)/main.cc
# If your main file is main.cpp, use:
# SOURCES = $(SRC_DIR)/main.cpp
# If Node has a separate implementation file (e.g., node.cc), add it:
# SOURCES = $(CORE_DIR)/node.cc $(SRC_DIR)/main.cc

# Object files (Adjust extension if using .cpp)
# --- Ensure this matches the extension used in SOURCES ---
OBJECTS = $(SOURCES:.cc=.o)
# If using .cpp, use:
# OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
TARGET = graph_app

# Default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJECTS)
# --- IMPORTANT: This line below MUST start with a TAB character ---
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Rule to compile source files into object files
# Adjust extension if using .cpp
# --- Ensure this rule's pattern matches the extension used in SOURCES ---
%.o: %.cc
# --- IMPORTANT: This line below MUST start with a TAB character ---
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@
# If using .cpp, use this rule instead (and ensure the command line starts with a TAB):
# %.o: %.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Clean target
clean:
# --- IMPORTANT: This line below MUST start with a TAB character ---
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean