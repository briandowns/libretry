#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "retry.h"

uint8_t
unreliable_operation(void)
{
    return rand() & 1;
}

int
main(void)
{
    retry_t *r = retry_new();
    retry_set_delay(r, 2000);
    retry_set_max_delay(r, 10);

    uint8_t ret = retry_do(r, unreliable_operation, NULL);

    printf("Final return code: %d\n", ret);

    retry_free(r);

    return 0;
}
