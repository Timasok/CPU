CC=g++
CFLAGS=-c -Wall
SRC_DIR = ASM/src/
OBJ_DIR = ASM/obj/

all: ASM

ASM: $(OBJ_DIR)main.o $(OBJ_DIR)funcs_ASM.o
	@$(CC) $(OBJ_DIR)main.o $(OBJ_DIR)funcs_ASM.o -o assembler

$(OBJ_DIR)main.o: $(SRC_DIR)main.cpp 
	@$(CC) $(SRC_DIR)main.cpp $(CFLAGS) -o $(OBJ_DIR)main.o

$(OBJ_DIR)funcs_ASM.o: $(SRC_DIR)funcs_ASM.cpp 
	@$(CC) $(SRC_DIR)funcs_ASM.cpp 	$(CFLAGS) -o $(OBJ_DIR)funcs_ASM.o

mkdir:
	mkdir $(OBJ_DIR) -p

clean:
	rm $(OBJ_DIR)*.o *.save stack