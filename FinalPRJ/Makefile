SRC_DIR ?= src
INC_DIR ?= include
#COMPILER := g++

SOURCES := $(notdir $(shell find $(SRC_DIR) -name *.cpp -or -name *.c))
OBJECTS = $(addprefix $(OBJ_DIR), $(SOURCES:$(suffix $(firstword $(SOURCES)))=.o))
DEPS = $(addprefix $(DEP_DIR), $(SOURCES:$(suffix $(firstword $(SOURCES)))=.d))
#-pthread
EXE = $(BIN_DIR)prog.exe
CXXFLAGS = -Iinclude -ggdb -I/usr/include/boost -lboost_thread -lrt -lpthread -lboost_system -lboost_timer -lboost_chrono # Compiler flags
# -lboost_filesystem -lboost_iostreams
# Making for host (Linux)
# > make ARCH=host
#ifeq (${ARCH},host)
ifeq ($(suffix $(firstword $(SOURCES))),.cpp) # If src files = .cpp
COMPILER = g++
OBJ_DIR = obj/#host/
DEP_DIR = deps/#host/
BIN_DIR = bin/#host/
STDFLAG = -std=c++17 # C++ std compiler flag for C++ 17
$(shell mkdir -p $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR))
endif
ifeq ($(suffix $(firstword $(SOURCES))),.c) # If src files = .c
COMPILER = gcc
OBJ_DIR = obj/#host/
DEP_DIR = deps/#host/
BIN_DIR = bin/#host/
STDFLAG = -std=c11 # C std compiler flag for C 17
$(shell mkdir -p $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR))
endif
#endif

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
	$(COMPILER) $(OBJECTS) -o $@ $(STDFLAG)$(CXXFLAGS) 

# Rule that describes how a .d (dependency) file is created from a .cpp file
${DEP_DIR}%.d: $(SRC_DIR)/%$(suffix $(firstword $(SOURCES)))
	$(COMPILER) -MT$(@:.d=.o) -MM $(STDFLAG) $(CXXFLAGS) $^ >> $@
	$(COMPILER) -MT$@ -MM $(STDFLAG) $(CXXFLAGS) $^ > $@

# Rule that describes how an .o (object) file is created from a .cpp file
${OBJ_DIR}%.o: $(SRC_DIR)/%$(suffix $(firstword $(SOURCES)))
	$(COMPILER) -I$(INC_DIR) $(STDFLAG) $(CXXFLAGS) -c $< -o $@

ifneq ($(filter-out clean format tidy,$(MAKECMDGOALS)),)
-include $(DEPS)
endif

all: $(EXE)

run: all
	@rm -rf $(SRC_DIR)/*.log
	clear
	@./$(EXE)
	


clean:
	@rm -rf $(OBJ_DIR)/*.o $(DEP_DIR)/*.d $(BIN_DIR)/*
	@rm -rf $(SRC_DIR)/*.log

.PHONY: all run clean
