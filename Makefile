CFLAGS =
CFLAGS += -g
CFLAGS += -O3
CFLAGS += -Wall
CFLAGS += --std=c99
CFLAGS += -Wno-unused-function
CFLAGS += -fno-inline

INC =
INC += -I../

.PHONY: all perftest

all: perftest

perftest:
	cd perftest; gcc $(CFLAGS) $(INC) perftest.c -o ../bin/perftest

