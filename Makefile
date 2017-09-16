
TARGET = shell

CC = gcc
CFLAGS = -g

SRCDIR = src
OBJDIR = obj
BINDIR = bin

OBJECTS = $(OBJDIR)/main.o

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(SRCDIR)/main.c


main.c: 





