#include "buffer_queue.h"
#include <stdio.h>

buffer_queue_t create() {
	buffer_queue_t bq = buffer_queue_create();

	buffer_queue_append(bq, (uv_buf_t) {.base = "ab", .len = 2});
	buffer_queue_append(bq, (uv_buf_t) {.base = "cd", .len = 2});
	buffer_queue_append(bq, (uv_buf_t) {.base = "ef", .len = 2});
	buffer_queue_append(bq, (uv_buf_t) {.base = "gh", .len = 2});

	return bq;
}
int main(int argc, char* argv[]) {
	buffer_queue_t bq = create();
	printf("length: %d\n", buffer_queue_length(bq));
	printf("offset of 'ab': %ld\n", buffer_queue_find(bq, (uv_buf_t){.base="ab", .len=2}));
	printf("offset of 'bc': %ld\n", buffer_queue_find(bq, (uv_buf_t) {.base="bc", .len=2}));
	printf("offset of 'cde': %ld\n", buffer_queue_find(bq, (uv_buf_t) {.base="cde", .len=3}));
	printf("offset of 'gh': %ld\n", buffer_queue_find(bq, (uv_buf_t) {.base="gh", .len=2}));
	printf("offset of 'cdefg': %ld\n", buffer_queue_find(bq, (uv_buf_t) {.base="cdefg", .len=5}));
	printf("offset of 'h': %ld\n", buffer_queue_find(bq, (uv_buf_t) {.base="h", .len=1}));

	uv_buf_t data = buffer_queue_slice_first(bq);
	printf("data of first:   [%.*s]\n", data.len, data.base);
	data = buffer_queue_slice(bq, 3);
	printf("data of slice 3: [%.*s]\n", data.len, data.base);
	data = buffer_queue_slice(bq, 1);
	printf("data of slice 1: [%.*s]\n", data.len, data.base);
	data = buffer_queue_slice(bq, 2);
	printf("data of slice 2: [%.*s]\n", data.len, data.base);
	data = buffer_queue_slice(bq, 5);
	printf("data of slice 5: [%.*s]\n", data.len, data.base);
	return 0;
}
