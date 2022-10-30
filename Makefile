ASM_SRC_DIR = ASM/src/
STK_SRC_DIR = lib/stack/src/
CPU_SRC_DIR = CPU/src/
LIB_DIR = lib/

I_FLAG := -I./ASM/include/ -I./CPU/include/ -I./lib/stack/include/ -I./inc/ -I./lib/

CC := g++
CFLAGS := $(I_FLAG)

SOURCES_ASM := $(ASM_SRC_DIR)main_ASM.cpp $(ASM_SRC_DIR)funcs_ASM.cpp $(ASM_SRC_DIR)debug_ASM.cpp $(ASM_SRC_DIR)inout_ASM.cpp
SOURCES_CPU := $(CPU_SRC_DIR)main_CPU.cpp $(CPU_SRC_DIR)funcs_CPU.cpp $(CPU_SRC_DIR)debug_CPU.cpp $(STK_SRC_DIR)stack_funcs.cpp $(STK_SRC_DIR)debug_funcs.cpp
SOURCES_TXT_LIB := $(LIB_DIR)text_funcs.cpp

EXECUTABLE_ASM := assembler
EXECUTABLE_CPU := cpu

all :
	$(CC) $(SOURCES_TXT_LIB) $(SOURCES_ASM) $(CFLAGS) -o $(EXECUTABLE_ASM)
	$(CC) $(SOURCES_CPU) $(CFLAGS) -o $(EXECUTABLE_CPU)