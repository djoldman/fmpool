NAME = example

OBJDIR = ../../bin

CFLAGS =
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += --std=c99
CFLAGS += -Wno-unused-function
CFLAGS += -fno-inline
CFLAGS += -I../..

all:
	$(CC) $(CFLAGS) $(NAME).c -o $(OBJDIR)/$(NAME)

clean:
	rm -fr $(OBJDIR)/$(NAME).o $(OBJDIR)/$(NAME) $(OBJDIR)/$(NAME).dSYM
