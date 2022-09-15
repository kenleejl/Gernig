CC = gcc
CXX = g++

DIR_GUARD = if not exist "$(@D)" mkdir "$(@D)"
SRC_DIR = src
OBJ_DIR = obj

OBJ = $(OBJ_DIR)/DllLoader/DllLoader.o $(OBJ_DIR)/DllLoader/MemoryModule.o
OBJ_PROGRAM = $(OBJ_DIR)/program.o

CFLAGS  = -Wall -g

INCLUDES = -Iinclude

.PHONY: all

all: DllLoader.exe program.exe

DllLoader.exe: $(OBJ)
	$(CC) -o $@ $^

program.exe: $(OBJ_PROGRAM)
	$(CXX) -o $@ $^ -static

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(DIR_GUARD)
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_GUARD)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)
