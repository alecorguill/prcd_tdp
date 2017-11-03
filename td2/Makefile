CC = gcc
CFLAGS = -std=c99 -O2
LDFLAGS =
DEBUG_FLAGS = -W -Wall -g
TEST = 
SRC = simulation_seq.c	
OBJS = $(SRC:.c=.o)

HEADER_DIR = header

vpath %.c src test
vpath %.h $(HEADER_DIR)

debug : CFLAGS += $(DEBUG_FLAGS)
debug : clean
debug : $(OBJS)


all : $(OBJS)

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<


clean:
	@rm -f *.o
