// @Author: terry.wuhao@gmail.com
#include "buffer_queue.h"
#include <stdlib.h>
#include <string.h>

typedef struct node_t {
	uv_buf_t        data;
	struct node_t*  next;
} node_t;

typedef struct buffer_queue {
	struct node_t*  head;
	struct node_t*  tail;
	size_t          offset; // 用于标识第一个 buffer 的偏移位置
	size_t          length;
} * buffer_queue_t;

buffer_queue_t buffer_queue_create() {
	return (buffer_queue_t)calloc(1, sizeof(struct buffer_queue));
}
void buffer_queue_append(buffer_queue_t bq, const uv_buf_t data) {
	node_t* n = (node_t*)malloc(sizeof(node_t));
	n->data = data;
	n->next = NULL;
	if(bq->head == NULL) {
		bq->head = n;
		bq->tail = n;
	}else{
		bq->tail->next = n;
		bq->tail = n;
	}
	bq->length += n->data.len;
}
size_t buffer_queue_length(buffer_queue_t bq) {
	return bq->length;
}
void buffer_queue_prepend(buffer_queue_t bq, const uv_buf_t data) {
	node_t* n = (node_t*)malloc(sizeof(node_t));
	n->data = data;
	n->next = bq->head;
	if(bq->tail == NULL) {
		bq->head = n;
		bq->tail = n;
	}else{
		bq->head = n;
	}
	bq->length += n->data.len;
}
uv_buf_t buffer_queue_slice_first(buffer_queue_t bq) {
	return buffer_queue_slice(bq, bq->head->data.len);
}
uv_buf_t buffer_queue_slice(buffer_queue_t bq, size_t size) {
	// 数据不足
	if(size > bq->length) {
		return (uv_buf_t) {.base = NULL, .len = 0};
	}
	uv_buf_t data;
	node_t*  node = bq->head;
	// 首个 buffer 特殊处理
	if(node->data.len - bq->offset > size) {
		data.base = node->data.base + bq->offset;
		data.len    = size;
		bq->offset += size;
		bq->length -= size;
		return data;
	}else if(node->data.len - bq->offset == size) {
		data.base = (char*)malloc(size);
		data.len  = size;
		memcpy(data.base, node->data.base + bq->offset, size);

		bq->offset = 0;
		bq->length-= size;
		bq->head = node->next;
		free(node);
		return data;
	}else{ // node->data.len < size
		data.base  = (char*)malloc(size);
		memcpy(data.base, node->data.base, node->data.len);
		data.len   = node->data.len;
		size      -= data.len;

		bq->offset = 0;
		bq->length-= node->data.len;
		bq->head   = node->next;
		free(node);
	}

	while(size > 0) {
		node = bq->head;
		if(node->data.len > size) {
			memcpy(data.base + data.len, node->data.base, size);
			data.len   += size;
			bq->offset += size;
			bq->length -= size;
			return data;
		}else if(node->data.len == size) {
			memcpy(data.base + data.len, node->data.base, size);
			data.len  += size;

			bq->offset = 0;
			bq->offset-= size;
			bq->head   = node->next;
			free(node);
			return data;
		}else{ // node->data.len < size
			memcpy(data.base + data.len, node->data.base, node->data.len);
			size      -= node->data.len;
			data.len  += node->data.len;
			bq->offset = 0;
			bq->length-= node->data.len;
			bq->head   = node->next;
			free(node);
		}
	}
	return data;
}
ssize_t buffer_queue_find(buffer_queue_t bq, const uv_buf_t delim) {
	ssize_t  l = 0;
	size_t   i = 0;
	node_t*  n = bq->head;
	char*    c = n->data.base;
	uv_buf_t data = {.base = NULL, .len = 0};
	if(bq->head == NULL) return -1;

	while(i < delim.len) {
		if(delim.base[i] == *c) {
			++ i; // 标记查找串位置
		}else{
			i=0;
		}
		++ l;
		if(c - n->data.base < n->data.len - 1) { // 本段继续
			++ c;
		}else if(n->next != NULL) { // 下一段
			n = n->next;
			c = n->data.base;
		}else if(i == delim.len) { // 恰好在结尾
			break;
		}else{ // 结尾，未找到
			return -1;
		}
	}
	return l;
}
void buffer_queue_destroy(buffer_queue_t bq) {
	node_t * n = bq->head, * o;
	while(n != NULL) {
		o = n;
		if(n->next != NULL) n = n->next;
		free(o);
	}
	free(bq);
}
