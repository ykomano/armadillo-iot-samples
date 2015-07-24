ifdef SDK_DIR
else
SDK_DIR=/path/to/libkii/KiiThingSDK
endif

CC = gcc
CFLAGS=-I$(SDK_DIR) -I$(SDK_DIR)/jansson
LDFLAGS=-L$(SDK_DIR) -L$(SDK_DIR)/jansson -lkii -ljansson

all:
	$(CC) register.c $(CFLAGS) $(LDFLAGS) -o register
