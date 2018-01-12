CC = gcc
CFLAGS = -std=c99 
LDFLAGS = -lm
DEBUG_FLAGS = -W -Wall -g
TEST = test_unitaire.c
SRC = 
OBJS = $(SRC:.c=.o)
DEBUG = 
TEST_DIRECTORY = test
HEADER_DIR = header
EXECS_TEST = $(basename $(TEST))

vpath %.c src test
vpath %.h $(HEADER_DIR)


ifeq ($(DEBUG),1)
	CFLAGS +=  $(DEBUG_FLAGS)
endif

parallel : CC = mpicc
parallel : CFLAGS += -DMPIFLAG
parallel : simulation.o $(OBJS)

sequentiel : life_seq.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

openmp : life_openmp.o $(OBJS)
	$(CC) -fopenmp -o $@ $^ $(LDFLAGS)

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

%.o : %.c
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

test : test_unitaire

test_unitaire : particule.o util.o test_unitaire.o 
	$(CC) -o $@ $^ $(LDFLAGS)


clean:
	@rm -f *.o $(EXECS_TEST) openmp sequentiel
