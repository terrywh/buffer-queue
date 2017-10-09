CFLAGS?= -O3
CFLAGS+= -std=c99 -fPIC
CC=gcc
AR?=ar

all: buffer_queue.o

%.o: %.c
	$(CC) ${CFLAGS} -c $^ -o $@
libbq.so: buffer_queue.o
	$(CC) -shared -o $@ $^
libbq.a: buffer_queue.o
	$(AR) rcs $@ $^

clean:
	rm -f *.o *.a *.so
buffer_queue_test: buffer_queue.o buffer_queue_test.o
	$(CC) $^ -o $@
test: buffer_queue_test
	./buffer_queue_test
