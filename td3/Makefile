CC = mpicc
CFLAGS = -std=c99 
LDFLAGS = -lm
DEBUG_FLAGS = -W -Wall -g
TEST = unit_test.c
SRC = util.c gemm.c
OBJS = $(SRC:.c=.o)
DEBUG = 
TEST_DIRECTORY = test
HEADER_DIR = header
EXECS_TEST = $(basename $(TEST))
EXEC = gemm_fox

vpath %.c src test
vpath %.h $(HEADER_DIR)

ifeq ($(DEBUG),1)
	CFLAGS +=  $(DEBUG_FLAGS)
endif

all : gemm_fox.o $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC) $^ 

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

test : test_unitaire

test_unitaire : $(OBJS) test_unitaire.o 
	$(CC) -o $@ $^ $(LDFLAGS)

check : unit_test
	echo 'unit_test'
clean:
	rm -f *.o $(EXEC) test_unitaire
