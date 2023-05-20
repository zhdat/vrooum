
PILOT_NAME = droitAuBut

CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -Wpedantic -g -I.
LDLIBS = -lm

SANITIZE=off
ifeq ($(SANITIZE),on)
CFLAGS += -fsanitize=address
endif

.PHONY: all clean distclean install try

DRIVERS_PATH = ../drivers

all: $(PILOT_NAME)

install: $(DRIVERS_PATH)/$(PILOT_NAME)

try: install
	cd .. ; ./GrandPrix

$(DRIVERS_PATH)/$(PILOT_NAME): $(PILOT_NAME)
	cp $(PILOT_NAME) $(DRIVERS_PATH)

$(PILOT_NAME): $(PILOT_NAME).o follow_line.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(PILOT_NAME).o: $(PILOT_NAME).c $(PILOT_NAME).h
	$(CC) $(CFLAGS) -c $(PILOT_NAME).c

follow_line.o: follow_line.c follow_line.h
	$(CC) $(CFLAGS) -c follow_line.c

distclean: clean
	rm -f $(PILOT_NAME)

clean:
	rm -f *~ *.o
