
TARGET = shell

CC = gcc
CFLAGS = -g

SRCDIR = src
OBJDIR = obj
BINDIR = bin

OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/parser.o $(OBJDIR)/write.o $(OBJDIR)/read.o

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/parser.h $(SRCDIR)/command.h \
		$(SRCDIR)/write.h $(SRCDIR)/read.h

$(OBJDIR)/parser.o: $(SRCDIR)/parser.c $(SRCDIR)/parser.h $(SRCDIR)/command.h

$(OBJDIR)/write.o: $(SRCDIR)/write.c

$(OBJDIR)/read.o: $(SRCDIR)/read.c $(SRCDIR)/read.h

clean:
	rm -f $(BINDIR)/$(TARGET) $(OBJECTS)

.PHONY: clean




