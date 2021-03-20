#include <os_internal.h>
#include "siint.h"
#include <assert.h>

// This whitespace is necessary for matching the assert












































s32 __osSiRawWriteIo(u32 devAddr, u32 data)
{
    assert_filename((devAddr & 3) == 0, "sirawwrite.c");
    if (__osSiDeviceBusy())
        return -1;
    IO_WRITE(devAddr, data);
    return 0;
}
