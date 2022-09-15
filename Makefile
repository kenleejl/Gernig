CC = gcc
CXX = g++

SRC_DIR = src
INCLUDES = -Iinclude

.PHONY: all

all: DllLoader.exe program.exe

DllLoader.exe:
	$(CXX) $(SRC_DIR)/DllLoader.cpp $(SRC_DIR)/MemoryModule.c -o $@ $(INCLUDES)

program.exe:
	$(CXX) $(SRC_DIR)/program.cpp -o $@
