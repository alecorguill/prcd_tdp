CC = gcc
CFLAGS = -std=c99 -O0 -Wno-unused-parameter
LDFLAGS = -fopenmp -lm
DEBUG_FLAGS = -W -Wall -g
TEST = perf.c
SRC = dgetf2_nopiv.c dgetrf_nopiv.c util.c
OBJS = $(SRC:.c=.o)

HEADER_DIR = header

vpath %.c src test
vpath %.h $(HEADER_DIR)

ifeq ($(DEBUG),1)
	CFLAGS +=  $(DEBUG_FLAGS)
endif


all : $(OBJS)

slave_lu: CC=mpicc
slave_lu: $(OBJS)  dgetrf_mpi_slave.o
	$(CC) -I$(HEADER_DIR) $(CFLAGS) $(LDFLAGS) -o $@ $^  

test_parallel: CC=mpicc
test_parallel: OBJS+=dgetrf_mpi_nopiv.c
test_parallel: test_mpi_unitaire
test_mpi_unitaire: $(OBJS) perf_mpi.c
	$(CC) -I$(HEADER_DIR) $(CFLAGS) $(LDFLAGS) -o $@ $^ 




test :  test_unitaire
test_unitaire : $(OBJS) perf.c 
	$(CC) -I$(HEADER_DIR) $(CFLAGS) $(LDFLAGS) -o $@ $^ 


%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

%.o : %.c
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

clean:
	@rm -f *.o test_unitaire *.csv slave_lu test_mpi_unitaire
