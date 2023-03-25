
PILOT_NAME = droitAuBut

CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -Wpedantic -g
LDLIBS = -lm
SRCDIR = .
OBJDIR = obj
BINDIR = .

SRCFILES = $(wildcard $(SRCDIR)/*.c)
OBJFILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))
DEPFILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.d,$(SRCFILES))

SANITIZE=off
ifeq ($(SANITIZE),on)
CFLAGS += -fsanitize=address
endif

.PHONY: all clean distclean install try

DRIVERS_PATH = ../drivers

EXEC = droitAuBut

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJFILES)
	$(CC) -o $@ $^ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

-include $(DEPFILES)

install: $(DRIVERS_PATH)/$(PILOT_NAME)

try: install
	cd .. ; ./GrandPrix

$(DRIVERS_PATH)/$(PILOT_NAME): $(PILOT_NAME)
	cp $(PILOT_NAME) $(DRIVERS_PATH)

$(PILOT_NAME): $(PILOT_NAME).c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


distclean: clean
	rm -f $(PILOT_NAME)

clean:
	rm -f *~ *.o
