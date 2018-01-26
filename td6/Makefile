CC = gcc
CFLAGS = -g -O0 -std=c99 
LDFLAGS = -lm
DEBUG_FLAGS = -W -Wall -g
TEST = test_unitaire.c
SRC = life_mpi.c
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

mpi : CC = mpicc
mpi : CFLAGS += -DMPIFLAG
mpi : $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)	

sequentiel : life_seq.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

openmp : CFLAGS += -fopenmp
openmp : life_openmp.o $(OBJS)
	$(CC) -fopenmp -o $@ $^ $(LDFLAGS)


pthread : life_pthread.o $(OBJS)
	$(CC) -lpthread -o $@ $^ $(LDFLAGS)

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

%.o : %.c
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

test : test_unitaire

test_unitaire : particule.o util.o test_unitaire.o 
	$(CC) -o $@ $^ $(LDFLAGS)


clean:
	@rm -f *.o *~ $(EXECS_TEST) openmp sequentiel mpi
