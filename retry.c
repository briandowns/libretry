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

#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "retry.h"

#define DEFAULT_RETRIES 5
#define BASE_DELAY_MS 100
#define MAX_DELAY_MS 5000

struct retry {
    uint64_t attempts;
    uint64_t retries;
    uint64_t delay;
    uint64_t max_delay;
};

/**
 * init_srand seeds the random number generator but will only do it once.
 */
static void
init_srand()
{
    static uint8_t called = 0;

    if (called == 0) {
        srand(time(NULL));
        called = 1;
    }
}

retry_t*
retry_new()
{
    init_srand();

    retry_t *r = (retry_t*)calloc(1, sizeof(retry_t));
    r->retries = DEFAULT_RETRIES;
    r->delay = BASE_DELAY_MS;
    r->max_delay = MAX_DELAY_MS;

    return r;
}

void
retry_set_retries(retry_t *r, const uint64_t retries)
{
    r->retries = retries;
}

void
retry_set_delay(retry_t *r, const uint64_t delay)
{
    r->delay = delay;
}

void
retry_set_max_delay(retry_t *r, const uint64_t delay)
{
    r->max_delay = delay * 1000;
}

uint8_t
retry_do(retry_t *r, const retry_func_t f, void *user_data)
{
    uint8_t ret = 0;

    while (r->attempts < r->retries) {
        if ((ret = f()) == 0) {
            break;
        }

        r->delay = r->delay << 1;
        if (r->delay > r->max_delay) {
            break;
        }

        // converts to nanoseconds
        usleep(r->delay * 1000);

        r->attempts++;
    }

    return ret;
}

void
retry_free(retry_t *r)
{
    if (r != NULL) {
        free(r);
    }
}