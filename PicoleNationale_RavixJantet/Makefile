CC       = gcc
RM       = rm
DOC      = doxygen
CPPFLAGS = -I./include
CFLAGS   = -Wall -Wextra -ansi -pedantic -Wpedantic -g
LDFLAGS  = -L ./lib -lm# Add the libraries you will use, e.g. -lm

.PHONY: all doc clean distclean

all: ./bin/PicoleNationale

./bin/PicoleNationale: ./src/main.o ./src/aStar.o ./src/follow_line.o ./src/hashSet.o ./src/list.o ./src/node.o ./src/priorityQueue.o# And others .o
	$(CC) $^ -o $@ $(LDFLAGS)

./src/aStar.o: ./src/aStar.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c 

./src/hashSet.o: ./src/hashSet.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

./src/list.o: ./src/list.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

./src/node.o: ./src/node.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

./src/priorityQueue.o: ./src/priorityQueue.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

./src/follow_line.o: ./src/follow_line.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

doc:
	-@$(DOC) doc/Doxyfile

clean:
	-@$(RM) ./src/*.o

distclean: clean
	-@$(RM) ./bin/PicoleNationale
