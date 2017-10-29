CC = gcc
CFLAGS = -std=c99 -W -Wall -g -O2
LDFLAGS =

TEST = driver.c perf_vecteur.c perf_matrice.c
TEST_OBJS = $(TEST:.c=.o)
SRC = util.c ddot.c dgemm.c perf.c
OBJS = $(SRC:.c=.o)
HEADER_DIR = header
EXEC_NAMES = driver perf_vecteur.c perf_matrice.c

vpath %c src test
vpath %h $(HEADER_DIR)

all : $(OBJS)

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -c $<

test : $(OBJS) $(TEST_OBJS)

clean:
	@rm -f *.o test_*
