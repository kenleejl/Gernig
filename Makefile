CC = gcc
CXX = g++

DIR_GUARD = @if not exist "$(@D)" mkdir "$(@D)"
SRC_DIR = src
OBJ_DIR = obj
OBJ_DIR_MAIN = $(OBJ_DIR)/DllLoader

SRC_MODULES = $(wildcard $(SRC_DIR)/modules/*.cpp)
OBJ_MODULES =  $(SRC_MODULES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

OBJ = $(OBJ_DIR_MAIN)/main.o $(OBJ_DIR_MAIN)/loader.o $(OBJ_DIR_MAIN)/MemoryModule.o $(OBJ_MODULES)
OBJ_PROGRAM = $(OBJ_DIR)/program.o

CFLAGS  = -Wall -g

INCLUDES = -Iinclude

.PHONY: all

all: loader.exe program.exe

loader.exe: $(OBJ)
	$(CXX) -o $@ $^

# Link with -static-libstdc++ when using std::cout, etc.
program.exe: $(OBJ_PROGRAM)
	$(CXX) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(DIR_GUARD)
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_GUARD)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)
