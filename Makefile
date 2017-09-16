
TARGET = shell

CC = gcc
CFLAGS = -g

SRCDIR = src
OBJDIR = obj
BINDIR = bin

OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/parser.o

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/parser.h $(SRCDIR)/command.h

$(OBJDIR)/parser.o: $(SRCDIR)/parser.c $(SRCDIR)/parser.h $(SRCDIR)/command.h

clean:
	rm shell $(OBJECTS)

.PHONY: clean




