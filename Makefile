CC = gcc-6
OBJS = main.out
CFLAGS = -std=c11
LFLAGS = -lallegro -lallegro_primitives -lallegro_main
DEPS = ../Vector/vector.c
main.out: main.c Rocket.h ../Vector/vector.h
	$(CC) $(CFLAGS) main.c $(DEPS) $(LFLAGS) -o $(OBJS)

clean:
	\rm *.out
