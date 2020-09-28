SRC_DIR ?= src
INC_DIR ?= include

SOURCES := $(notdir $(shell find $(SRC_DIR) -name *.cpp -or -name *.c))
OBJECTS = $(addprefix $(OBJ_DIR), $(SOURCES:$(suffix $(firstword $(SOURCES)))=.o))
DEPS = $(addprefix $(DEP_DIR), $(SOURCES:$(suffix $(firstword $(SOURCES)))=.d))

EXE = $(BIN_DIR)prog.exe
CXXFLAGS = -Wall -Wextra -Iinclude -I/usr/include/boost -ggdb -pthread # Compiler flags

# Making for host (Linux)
# > make ARCH=host
ifeq (${ARCH},host)
ifeq ($(suffix $(firstword $(SOURCES))),.cpp) # If src files = .cpp
COMPILER = g++
OBJ_DIR = obj/host/
DEP_DIR = deps/host/
BIN_DIR = bin/host/
STDFLAG = -std=c++17 # C++ std compiler flag for C++ 17
$(shell mkdir -p $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR))
endif
ifeq ($(suffix $(firstword $(SOURCES))),.c) # If src files = .c
COMPILER = gcc
OBJ_DIR = obj/host/
DEP_DIR = deps/host/
BIN_DIR = bin/host/
STDFLAG = -std=c11 # C std compiler flag for C 17
$(shell mkdir -p $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR))
endif
endif

# Making for target (Raspberry PI)
# > make ARCH=target
ifeq (${ARCH},target)
ifeq ($(suffix $(firstword $(SOURCES))),.cpp) # If src files = .cpp
COMPILER = arm-linux-gnueabihf-g++
OBJ_DIR = obj/target/
DEP_DIR = deps/target/
BIN_DIR = bin/target/
STDFLAG = -std=c++17 # C++ std compiler flag for C++ 17
$(shell mkdir -p $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR))
endif
ifeq ($(suffix $(firstword $(SOURCES))),.c) # If src files = .c
COMPILER = arm-linux-gnueabihf-gcc
OBJ_DIR = obj/target/
DEP_DIR = deps/target/
BIN_DIR = bin/target/
STDFLAG = -std=c11
$(shell mkdir -p $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR))
endif
endif

$(EXE): $(DEPS) $(OBJECTS) # << Check new dependency
	$(COMPILER) $(CXXFLAGS) $(STDFLAG) -o $@ $(OBJECTS)

# Rule that describes how a .d (dependency) file is created from a .cpp file
${DEP_DIR}%.d: $(SRC_DIR)/%$(suffix $(firstword $(SOURCES)))
	$(COMPILER) -MT$(@:.d=.o) -MM $(CXXFLAGS) $(STDFLAG) $^ >> $@
	$(COMPILER) -MT$@ -MM $(CXXFLAGS) $(STDFLAG) $^ > $@

# Rule that describes how an .o (object) file is created from a .cpp file
${OBJ_DIR}%.o: $(SRC_DIR)/%$(suffix $(firstword $(SOURCES)))
	$(COMPILER) $(CXXFLAGS) $(STDFLAG) -I$(INC_DIR) -c $< -o $@

ifneq ($(filter-out clean format tidy,$(MAKECMDGOALS)),)
-include $(DEPS)
endif

all: $(EXE)

run: all
	clear
	@./$(EXE)

clean:
	@rm -rf $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR)

.PHONY: all run clean
