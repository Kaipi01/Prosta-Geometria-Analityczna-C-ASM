CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC -Ilib -Iinc
LDFLAGS = -lm -lraylib
VPATH = lib

all: main

main: main.o libgeomath.so libsavefile.so libparsepoint.so libdrawmath.so libutils.so
	$(CC) -o main main.o -L. -Wl,-rpath,'$$ORIGIN' -lsavefile -lparsepoint -ldrawmath -lutils $(LDFLAGS)

libgeomath.so: geo_math.o
	$(CC) -shared -o libgeomath.so geo_math.o

libsavefile.so: save_file.o
	$(CC) -shared -o libsavefile.so save_file.o

libparsepoint.so: parse_str_point.o
	$(CC) -shared -o libparsepoint.so parse_str_point.o 

libdrawmath.so: draw_math.o geo_math.o geo_math_extend.o
	$(CC) -shared -o libdrawmath.so draw_math.o geo_math.o geo_math_extend.o

libutils.so: utils.o
	$(CC) -shared -o libutils.so utils.o

clean:
	rm -f *.o
