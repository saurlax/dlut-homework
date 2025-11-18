CC=gcc
CFLAGS=-g -Wall
BIN_DIR=bin

OBJS=$(BIN_DIR)/ipc.o

all: $(BIN_DIR) $(BIN_DIR)/supplier $(BIN_DIR)/smoker

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/ipc.o: ipc.c ipc.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -c ipc.c -o $(BIN_DIR)/ipc.o

$(BIN_DIR)/supplier: supplier.c ipc.h $(BIN_DIR)/ipc.o
	$(CC) $(CFLAGS) supplier.c $(BIN_DIR)/ipc.o -o $(BIN_DIR)/supplier

$(BIN_DIR)/smoker: smoker.c ipc.h $(BIN_DIR)/ipc.o
	$(CC) $(CFLAGS) smoker.c $(BIN_DIR)/ipc.o -o $(BIN_DIR)/smoker

clean:
	rm -rf $(BIN_DIR)