CC = gcc
CFLAGS = -std=gnu99 
LDFLAGS = -lm
DEBUG_FLAGS = -W -Wall -g
TEST = test_unitaire.c
SRC = util.c
OBJS = $(SRC:.c=.o)
DEBUG = -g -O0
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
mpi : life_mpi.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)	

sequentiel : life_seq.o 
	$(CC) -o $@ $^ $(LDFLAGS)

openmp : CFLAGS += -fopenmp
openmp : life_openmp.o $(OBJS)
	$(CC) -fopenmp -o $@ $^ $(LDFLAGS)


pthread : CFLAGS += -pthread
pthread : life_pthread.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

%.o : %.c
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

test : test_unitaire

test_unitaire : particule.o util.o test_unitaire.o 
	$(CC) -o $@ $^ $(LDFLAGS)


clean:
	@rm -f *.o *~ $(EXECS_TEST) openmp sequentiel mpi pthread
