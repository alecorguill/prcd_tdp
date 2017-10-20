CC = gcc
CFLAGS = -W -Wall -g -O2
LDFLAGS =

TEST = driver.c
TEST_OBJS = $(TEST:.c=.o)
SRC = util.c ddot.c
OBJS = $(SRC:.c=.o)
HEADER_DIR = header
EXEC_NAME = exec

vpath %c src test
vpath %h $(HEADER_DIR)

all : $(OBJS)

%.o : %.c %.h
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -o $@ -c $<

test : util.o ddot.o $(TEST_OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC_NAME) $^

clean :
	@rm -f *.o $(EXEC_NAME)
