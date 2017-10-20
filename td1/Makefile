CC = gcc
CFLAGS = -W -Wall -g -O2
LDFLAGS =

TEST = driver.c
TEST_OBJS = $(TEST:.c=.o)
SRC = $(TEST) + util.c ddot.c
OBJS = $(SRC:.c=.o)
HEADER_DIR = header
HEADER_FILES = $(foreach d, $(HEADER_DIRS), $d/*)
HEADER_FLAGS = $(foreach d, $(HEADER_DIRS), -I$d)

vpath %c src test
vpath %h $(HEADER_DIR)

all : $(OBJS)

%.o : %.c %.h
	$(CC) -Iheader $(CFLAGS) -o $@ -c $<

test : $(TEST_OBJS) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean :
	@rm *.o
