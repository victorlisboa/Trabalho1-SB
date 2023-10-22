CXX = g++
CFLAGS = -std=c++17 -Wall -Wextra -g

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

PROGRAMS = montador simulador

all: setup $(PROGRAMS)

setup:
	mkdir -p $(OBJ_DIR)

montador: $(OBJ_DIR)/montador.o $(OBJ_DIR)/preprocessador.o $(OBJ_DIR)/utils.o
	$(CXX) $(CFLAGS) $^ -o $@

simulador: $(OBJ_DIR)/simulador.o $(OBJ_DIR)/utils.o
	$(CXX) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(PROGRAMS)

.PHONY: all clean
