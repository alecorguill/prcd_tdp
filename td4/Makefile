CC = gcc
CFLAGS = -std=c99 -O0 -Wno-unused-parameter
LDFLAGS = -fopenmp -lm
DEBUG_FLAGS = -W -Wall -g
TEST = perf.c
SRC = dgetf2_nopiv.c util.c
OBJS = $(SRC:.c=.o)

HEADER_DIR = header

vpath %.c src test
vpath %.h $(HEADER_DIR)

ifeq ($(DEBUG),1)
	CFLAGS +=  $(DEBUG_FLAGS)
endif


all : $(OBJS)

test :  test_unitaire

test_unitaire : $(OBJS) perf.c 
	$(CC) -I$(HEADER_DIR) $(CFLAGS) $(LDFLAGS) -o $@ $^ 

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<
clean:
	@rm -f *.o test_unitaire
