#ifndef BQ_BUFFER_QUEUE_H
#define BQ_BUFFER_QUEUE_H


#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bq_buffer {
	void*  next;
	size_t len;
	char   data[0];
}* bq_buffer_t;

// =============================================================================

typedef struct bq_queue* bq_queue_t;
bq_buffer_t bq_alloc(size_t size);
bq_buffer_t bq_from(char* data);
bq_queue_t  bq_create();
void        bq_destroy(bq_queue_t bq);
void        bq_append(bq_queue_t bq, bq_buffer_t data);
size_t      bq_length(bq_queue_t bq);
// extract the first buffer or last
bq_buffer_t bq_shift(bq_queue_t bq);
// slice the buffers of size bytes from the beginning of the queue and return the sliced part
bq_buffer_t bq_slice(bq_queue_t bq, size_t size);
// find the delimiter and return the end position where it was found (including the delimiter)
ssize_t     bq_find(bq_queue_t bq, const char* delim, size_t delen);
void        bq_free(void* data);
#ifdef __cplusplus
}
#endif

#endif
