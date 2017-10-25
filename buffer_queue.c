// @Author: terry.wuhao@gmail.com
#include "buffer_queue.h"
#include <stdlib.h>
#include <string.h>

typedef struct bq_queue {
	bq_buffer_t     head;
	size_t          len;
	bq_buffer_t     tail;
} * bq_queue_t;

bq_buffer_t bq_alloc(size_t size) {
	// TODO 靠齐内存大小
	bq_buffer_t data = (bq_buffer_t)malloc(sizeof(bq_buffer_t) + size);
	data->len = size;
	return data;
}
void bq_free(void* data) {
	free(data);
}
bq_queue_t bq_create() {
	bq_queue_t q = (bq_queue_t)bq_alloc(sizeof(bq_buffer_t));
	q->head = NULL;
	q->len  = 0;
	return q;
}
void bq_append(bq_queue_t bq, bq_buffer_t data) {
	data->next = NULL;
	if(bq->tail != NULL) {
		bq->tail->next = data;
		bq->tail = data;
	}else{
		bq->head = data;
		bq->tail = data;
	}
	bq->len += data->len;
}
size_t bq_length(bq_queue_t bq) {
	return bq->len;
}
bq_buffer_t bq_shift(bq_queue_t bq) {
	bq_buffer_t h = bq->head;
	bq->head = h->next;
	bq->len -= h->len;
	return h;
}
bq_buffer_t bq_slice(bq_queue_t bq, size_t size) {
	// 数据不足
	if(size > bq->len) {
		return NULL;
	}
	bq_buffer_t item;
	bq_buffer_t node = bq->head;
	// 首个 buffer 特殊处理优化效率
	if(node->len > size) {
		item = node;
		node = bq_alloc(node->len - size);
		memcpy(node->data, item->data + size, node->len);
		node->next = item->next;
		item->len  = size;
		bq->len   -= size;
		bq->head   = node;
		return item;
	}else if(node->len == size) {
		bq->head = node->next;
		bq->len -= size;
		return node;
	}else{ // node->len < size
		item = bq_alloc(size);
		memcpy(item->data, node->data, node->len);
		item->len = node->len;

		size    -= node->len;
		bq->len -= node->len;
		bq->head = node->next;
		bq_free(node);
	}

	while(size > 0) {
		node = bq->head;
		if(node->len > size) {
			memcpy(item->data + item->len, node->data, size);
			item->len += size;
			bq->head   = bq_alloc(node->len - size);
			memcpy(bq->head->data, node->data + size, bq->head->len);
			bq->head->next = node->next;
			bq->len   -= size;
			bq_free(node);
			return item;
		}else if(node->len == size) {
			memcpy(item->data + item->len, node->data, size);
			item->len += size;
			bq->len   -= size;
			bq->head   = node->next;
			bq_free(node);
			return item;
		}else{ // node->len < size
			memcpy(item->data + item->len, node->data, node->len);
			size      -= node->len;
			item->len += node->len;
			bq->len   -= node->len;
			bq->head   = node->next;
			bq_free(node);
		}
	}
	return item;
}
ssize_t bq_find(bq_queue_t bq, const char* delim, size_t delen) {
	if(bq->head == NULL) return -1;

	ssize_t  l = 0;
	size_t   i = 0;
	bq_buffer_t  n = bq->head;
	char*    c = n->data;

	while(i < delen) {
		if(delim[i] == *c) {
			++ i; // 标记查找串位置
		}else{
			i=0;
		}
		++ l;
		if(c - n->data < n->len - 1) { // 本段继续
			++ c;
		}else if(n->next != NULL) { // 下一段
			n = n->next;
			c = n->data;
		}else if(i == delen) { // 恰好在结尾
			break;
		}else{ // 结尾，未找到
			return -1;
		}
	}
	return l;
}
void bq_destroy(bq_queue_t bq) {
	bq_buffer_t n = bq->head, o;
	while(n != NULL) {
		o = n;
		n = n->next;
		free(o);
	}
	free(bq);
}
