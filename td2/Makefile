CC = gcc
CFLAGS = -std=c99 -O2
LDFLAGS =
DEBUG_FLAGS = -W -Wall -g
TEST = test_unitaire.c
SRC = simulation_seq.c particule.c simulation.c util.c
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

test : test_unitaire

test_unitaire : $(OBJS) test_unitaire.o 
	$(CC) $(LDFLAGS) -o $@ $^


clean:
	@rm -f *.o
