#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "retry.h"

// Simulate an operation that may fail 50% of the time.
// Returns 0 for success or for fail
uint8_t
unreliable_operation(void)
{
    // return rand() & 1;
    return 1;
}

int
main(void)
{
    retry_t *r = retry_new();
    retry_set_delay(r, 2);
    retry_set_max_delay(r, 10);

    uint8_t ret = retry_do(r, unreliable_operation, NULL);
}
