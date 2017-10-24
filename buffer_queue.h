#ifndef BQ_BUFFER_QUEUE_H
#define BQ_BUFFER_QUEUE_H


#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UV_H
// 以下部分代码摘自 libuv 附相关许可证
// =============================================================================
/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#if defined(_MSC_VER) && _MSC_VER < 1600
# include "stdint-msvc2008.h"
#else
# include <stdint.h>
#endif

#if defined(_WIN32)
/**
 * It should be possible to cast uv_buf_t[] to WSABUF[]
 * see http://msdn.microsoft.com/en-us/library/ms741542(v=vs.85).aspx
 */
typedef struct uv_buf_t {
	ULONG len;
	char* base;
} uv_buf_t;
#else
typedef struct uv_buf_t {
	char* base;
	size_t len;
} uv_buf_t;
#endif

#endif
// =============================================================================

typedef struct buffer_queue * buffer_queue_t;
uv_buf_t buffer_create(size_t size);
void buffer_destroy(uv_buf_t buf);
buffer_queue_t buffer_queue_create();
uv_buf_t buffer_queue_buffer(size_t size);
void buffer_queue_append(buffer_queue_t bq, const uv_buf_t data);
void buffer_queue_prepend(buffer_queue_t bq, const uv_buf_t data);
size_t   buffer_queue_length(buffer_queue_t bq);
// slice the buffers of size bytes from the beginning of the queue and return the sliced part
uv_buf_t buffer_queue_slice(buffer_queue_t bq, size_t size);
// slice the first buffer in the queue and return it
uv_buf_t buffer_queue_slice_first(buffer_queue_t bq);
// find the delimiter and return the end position where it was found (including the delimiter)
ssize_t  buffer_queue_find(buffer_queue_t bq, const uv_buf_t delim);
void buffer_queue_destroy(buffer_queue_t bq);

#ifdef __cplusplus
}
#endif

#endif
