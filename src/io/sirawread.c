#include <os_internal.h>
#include "siint.h"
#include <assert.h>

// This whitespace is necessary for matching the assert











































s32 __osSiRawReadIo(u32 devAddr, u32 *data)
{
    assert((devAddr & 3) == 0);
    assert(data != NULL);
    if (__osSiDeviceBusy())
        return -1;
    *data = IO_READ(devAddr);
    return 0;
}
