CC = gcc
CFLAGS = -W -Wall -g -O2
LDFLAGS =

TEST = perf.c driver.c
TEST_OBJS = $(TEST:.c=.o)
SRC = util.c ddot.c
OBJS = $(SRC:.c=.o)
HEADER_DIR = header
EXEC_NAMES = perf driver

vpath %c src test
vpath %h $(HEADER_DIR)

all : $(OBJS)

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -o $@ -c $<

test : $(OBJS) $(TEST_OBJS) $(EXEC_NAMES)

%: %.o 
	$(CC) -I$(HEADER_DIR) $(CFLAGS) $(OBJS) $^ -o test_$@	

clean:
	@rm -f *.o test_*
