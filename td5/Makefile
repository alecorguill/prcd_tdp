CC = gcc
CFLAGS = -std=c99 
LDFLAGS = -lm
DEBUG_FLAGS = -W -Wall -g
TEST = test_unitaire.c
SRC = particule.c util.c
EXEC = simul_seq
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
	$(CC) -o $@ $^ $(LDFLAGS)	
	@cp $@ test/

sequentiel : simulation_seq.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

sequentiel_bc : simulation_bc.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)


%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

%.o : %.c
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

test : test_unitaire

test_unitaire : particule.o util.o test_unitaire.o 
	$(CC) -o $@ $^ $(LDFLAGS)


clean:
	@rm -f *.o $(EXECS_TEST) parallel sequentiel
