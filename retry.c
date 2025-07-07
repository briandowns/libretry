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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "retry.h"

#define DEFAULT_RETRIES 5
#define BASE_DELAY_MS 100  // base delay in milliseconds
#define MAX_DELAY_MS 5000  // max backoff delay in milliseconds

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

struct retry {
    uint64_t attempts;
    uint64_t retries;
    uint64_t delay;
    uint64_t backoff_delay;
    uint64_t max_jitter;
};

static void
init_srand()
{
    static uint8_t called = 0;

    if (called == 0) {
        srand(time(NULL));
        called = 1;
    }
}

// Sleep for the specified number of milliseconds
void
sleep_ms(int milliseconds)
{
    // usleep takes microseconds
    usleep(milliseconds * 1000);
}

retry_t*
retry_new()
{
    init_srand();

    retry_t *r = (retry_t*)calloc(1, sizeof(retry_t));
    r->retries = DEFAULT_RETRIES;
    r->delay = BASE_DELAY_MS;
    r->backoff_delay = MAX_DELAY_MS;

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
    r->delay = delay * 1000;
}

void
retry_set_max_delay(retry_t *r, const uint64_t delay)
{
    r->backoff_delay = delay * 1000;
}

uint8_t
retry_do(retry_t *r, const retry_func_t f, void *user_data)
{
    uint8_t ret = 0;

    while (r->attempts < r->retries) {
        printf("Attempt #%lu...\n", r->attempts + 1);

        ret = f();
        if (ret == 0) {
            printf("Operation succeeded!\n");
            break;
        }

        // Calculate exponential backoff
        // int delay = r->delay * (1 << r->attempts);  // exponential backoff: base * 2^attempt
        // if (delay > r->backoff_delay) {
        //     delay = r->backoff_delay;
        // }

        // Add jitter (randomized delay between 0.5x and 1.0x of calculated delay)
        // int jitter = (rand() % (delay / 2 + 1));  // add up to 50% jitter
        // delay = delay / 2 + jitter;

        printf("Shifted delay: %lu, Retries: %lu\n", (r->delay<<1), r->retries);

        r->delay = r->delay << 1;
        printf("delay: %lu\n", r->delay);

        printf("Operation failed. Retrying in %lu ms...\n", r->delay);
        sleep_ms(r->delay);

        r->attempts++;
    }

    if (ret != 0) {
        printf("Operation failed after %lu attempts.\n", r->attempts);
    }

    return 0;
}

void
retry_free(retry_t *r)
{
    if (r != NULL) {
        free(r);
    }
}