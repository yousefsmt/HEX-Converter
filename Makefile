CC=gcc
C_FLAGS=-O2 -Wall -Werror -Wextra
INC_DIR=include
SRC_DIRC=src
SRCS=$(SRC_DIRC)/parser.c $(SRC_DIRC)/base_con.c \
	 $(SRC_DIRC)/log.c $(SRC_DIRC)/linked_list.c \
	 $(SRC_DIRC)/main.c
EXEC_NAME=hexconvert

default: all

install: all
	sudo cp $(EXEC_NAME) /usr/local/bin
all:
	$(CC) $(C_FLAGS) -I$(INC_DIR) $(SRCS) -o $(EXEC_NAME)
clean:
	rm -f $(EXEC_NAME)