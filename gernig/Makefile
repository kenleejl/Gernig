ifdef m32
BIN_SUF = 32
LDFLAGS = -static
else
BIN_SUF = 64
endif

CC = $(C_SUF)gcc -m$(BIN_SUF)
CXX = $(C_SUF)g++ -m$(BIN_SUF)


DIR_GUARD = @if not exist "$(@D)" mkdir "$(@D)"
SRC_DIR = src
OBJ_DIR = obj
OBJ_DIR_MAIN = $(OBJ_DIR)/DllLoader

SRC_MODULES = $(wildcard $(SRC_DIR)/modules/*.cpp)
OBJ_MODULES =  $(SRC_MODULES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%$(BIN_SUF).o)

OBJ = $(OBJ_DIR_MAIN)/main$(BIN_SUF).o $(OBJ_DIR_MAIN)/loader$(BIN_SUF).o $(OBJ_DIR_MAIN)/MemoryModule$(BIN_SUF).o $(OBJ_MODULES)
OBJ_PROGRAM = $(OBJ_DIR)/program$(BIN_SUF).o

CFLAGS  = -Wall -g -pthread

INCLUDES = -Iinclude

.PHONY: all

all: loader$(BIN_SUF).exe program$(BIN_SUF).exe

loader$(BIN_SUF).exe: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Link with -static-libstdc++ when using std::cout, etc.
program$(BIN_SUF).exe: $(OBJ_PROGRAM)
	$(CXX) -o $@ $^ 

$(OBJ_DIR)/%$(BIN_SUF).o: $(SRC_DIR)/%.cpp
	$(DIR_GUARD)
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDES)

$(OBJ_DIR)/%$(BIN_SUF).o: $(SRC_DIR)/%.c
	$(DIR_GUARD)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)
