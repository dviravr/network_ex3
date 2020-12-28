CC=gcc
AR=ar
FLAGS= -Wall
OBJECTS_LIB=measure.o sender.o
OBJECTS_MAIN=main.o

all: sender measure

measure: measure.o
	$(CC) $(FLAGS) measure.c -o measure

sender: sender.o
	$(CC) $(FLAGS) sender.c -o sender

clean:
	rm -f *.o *.a *.so sender measure

.PHONY: clean all
