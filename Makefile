CC = gcc-7
OBJS = main.out
CFLAGS = -std=c11
LFLAGS = -lallegro -lallegro_primitives -lm
DEPS = ../c-vector-lib/vector.c
main.out: main.c Rocket.h ../c-vector-lib/vector.h
	$(CC) $(CFLAGS) main.c $(DEPS) $(LFLAGS) -o $(OBJS)

clean:
	\rm *.out
