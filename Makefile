
TARGET = shell

CC = gcc
CFLAGS = -g

SRCDIR = src
OBJDIR = obj
BINDIR = bin

OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/parser.o $(OBJDIR)/write.o $(OBJDIR)/read.o $(OBJDIR)/execute.o

target: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@

noprompt: CFLAGS += -DNO_PROMPT
noprompt: target

debugparser: CFLAGS += -DDEBUG_PARSER
debugparser: target

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/parser.h $(SRCDIR)/command.h \
		$(SRCDIR)/write.h $(SRCDIR)/read.h $(SRCDIR)/execute.h

$(OBJDIR)/parser.o: $(SRCDIR)/parser.c $(SRCDIR)/parser.h $(SRCDIR)/command.h

$(OBJDIR)/write.o: $(SRCDIR)/write.c

$(OBJDIR)/read.o: $(SRCDIR)/read.c $(SRCDIR)/read.h

$(OBJDIR)/execute.o: $(SRCDIR)/execute.c $(SRCDIR)/write.h $(SRCDIR)/command.h

clean:
	rm -f $(BINDIR)/$(TARGET) $(OBJECTS)

.PHONY: target noprompt debugparser clean




