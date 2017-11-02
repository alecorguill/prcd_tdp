CC = icc
CFLAGS = -std=c99 -O2 -Wno-unused-parameter
LDFLAGS = -fopenmp -mkl
DEBUG_FLAGS = -W -Wall -g
TEST = driver.c perf_vecteur.c perf_matrice.c
SRC = util.c cblas.c blas.c perf.c
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

test : test_perf_vecteur test_driver test_perf_matrice

test_perf_vecteur : $(OBJS) perf_vecteur.o 
	$(CC) $(LDFLAGS) -o $@ $^

test_driver : driver.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

test_perf_matrice : perf_matrice.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	@rm -f *.o test_* test/*.csv
