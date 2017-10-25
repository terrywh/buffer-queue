CFLAGS?= -O3
CC=gcc
AR?=ar

.PHONY: all test clean
all: buffer_queue.o

%.o: %.c
	$(CC) ${CFLAGS} -std=c99 -fPIC -c $^ -o $@
libbq.so: buffer_queue.o
	$(CC) -shared -o $@ $^
libbq.a: buffer_queue.o
	$(AR) rcs $@ $^

clean:
	rm -f *.o *.a *.so buffer_queue_test

buffer_queue_test: buffer_queue.o buffer_queue_test.o
	$(CC) $^ -o $@

test: CFLAGS = -g -O0
test: buffer_queue_test
	./buffer_queue_test
