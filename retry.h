/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef __RETRY_H
#define __RETRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint8_t (*retry_func_t)(void);
typedef struct retry retry_t;

/**
 * retry_new creates a new value of type rety_r pointer. Freeing this memory
 * is the responsibility of the caller.
 */
retry_t*
retry_new();

/**
 * retry_set_retries sets the retry threshold with the given value. 
 */
void
retry_set_retries(retry_t *r, const uint64_t retries);

/**
 * retry_set_delay sets the delay threshold with the given value. The expected
 * value is in milliseconds.
 */
void
retry_set_delay(retry_t *r, const uint64_t delay);

/**
 * retry_set_max_delay sets the delay threshold with the given value. The
 * expected value is in milliseconds.
 */
void
retry_set_max_delay(retry_t *r, const uint64_t delay);

uint8_t
retry_do(retry_t *r, const retry_func_t f, void *user_data);

/**
 * retry_free frees the memory for the given retry value.
 */
void
retry_free(retry_t *r);

#ifdef __cplusplus
}
#endif

#endif /** __RETRY_H */
