# Compiler and compiler flags
CXX = g++
CFLAGS = -std=c++17 -Wall -Wextra -g

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

# Source files and target executable
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

EXECUTABLE = montador

# Targets
all: setup $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

setup:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)

.PHONY: all clean
