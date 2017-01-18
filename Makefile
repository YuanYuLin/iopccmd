
TARGET = iopccmd.elf

CFLAGS += -I$(SDKSTAGE)/usr/include/iopccommon
CFLAGS += -I$(SDKSTAGE)/usr/include/
CFLAGS += -I./include/

LDFLAGS += -L$(SDKSTAGE)/usr/lib/ 
LDFLAGS += -lc -lpthread -lrt
LDFLAGS += -liopccommon -liopccmd_client

SRC = 
SRC += main.c

include Makefile.include.app
