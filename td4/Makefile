CC = gcc
CFLAGS = -std=c99 -O2 -Wno-unused-parameter
LDFLAGS = -fopenmp 
DEBUG_FLAGS = -W -Wall -g
TEST = perf.c
SRC = dgetf2_nopiv.c util.c
OBJS = $(SRC:.c=.o)

HEADER_DIR = header

vpath %.c src test
vpath %.h $(HEADER_DIR)

debug : CFLAGS += $(DEBUG_FLAGS)
debug : clean
debug : $(OBJS)


all : $(OBJS)

test :  test_unitaire

test_unitaire : $(OBJS) perf.c 
	$(CC) -I$(HEADER_DIR) -o $@ $^  $(LDFLAGS)

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<
clean:
	@rm -f *.o test_unitaire
