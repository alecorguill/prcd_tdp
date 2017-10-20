CC = gcc
CFLAGS = -W -Wall -g -O2
LDFLAGS =

SRC = util.c ddot.c driver.c
OBJS = $(SRC:.c=.o)
HEADER_DIR = header
HEADER_FILES = $(foreach d, $(HEADER_DIRS), $d/*)

vpath %c src test

all : $(OBJS)

%.o : %.c
	$(CC) -I$(HEADER_DIR) $(CFLAGS) -o $@ -c $<
clean :
	@rm *.o
