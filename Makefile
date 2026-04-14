CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -O2
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/image_processor.exe

all: prepare $(TARGET)

prepare:
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	if not exist assets mkdir assets

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(TARGET) del $(TARGET)

run: all
	$(TARGET)
