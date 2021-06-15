#include <os_internal.h>
#include "piint.h"
#include <assert.h>

// This whitespace is necessary for matching the assert




















































s32 osPiRawReadIo(u32 devAddr, u32 *data)
{
    register u32 stat;
    assert(data != 0);
    WAIT_ON_IOBUSY(stat);
    *data = IO_READ((u32)osRomBase | devAddr);
    return 0;
}
