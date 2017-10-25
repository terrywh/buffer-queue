#include "buffer_queue.h"
#include <stdio.h>
#include <string.h>

bq_queue_t create() {
	bq_queue_t  bq = bq_create();
	bq_buffer_t bi;

	bi = bq_alloc(2); memcpy(bi->data, "ab", 2); bq_append(bq, bi);
	bi = bq_alloc(2); memcpy(bi->data, "cd", 2); bq_append(bq, bi);
	bi = bq_alloc(2); memcpy(bi->data, "ef", 2); bq_append(bq, bi);
	bi = bq_alloc(2); memcpy(bi->data, "gh", 2); bq_append(bq, bi);

	return bq;
}
int main(int argc, char* argv[]) {
	bq_queue_t bq = create();
	printf("length: %d\n", bq_length(bq));
	printf("offset of 'ab': %ld\n", bq_find(bq, "ab", 2));
	printf("offset of 'bc': %ld\n", bq_find(bq, "bc", 2));
	printf("offset of 'cde': %ld\n", bq_find(bq, "cde", 3));
	printf("offset of 'gh': %ld\n", bq_find(bq, "gh", 2));
	printf("offset of 'cdefg': %ld\n", bq_find(bq, "cdefg", 5));
	printf("offset of 'h': %ld\n", bq_find(bq, "h", 1));

	bq_buffer_t item = bq_shift(bq);
	printf("data of first:   (%d)[%.*s]\n", bq_length(bq), item->len, item->data);
	item = bq_slice(bq, 3);
	printf("data of slice 3: (%d)[%.*s]\n", bq_length(bq), item->len, item->data);
	item = bq_slice(bq, 1);
	printf("data of slice 1: (%d)[%.*s]\n", bq_length(bq), item->len, item->data);
	item = bq_slice(bq, 2);
	printf("data of slice 2: (%d)[%.*s]\n", bq_length(bq), item->len, item->data);
	item = bq_slice(bq, 5);
	printf("data of slice 5: (%d){%08x}\n", bq_length(bq), item);
	return 0;
}
