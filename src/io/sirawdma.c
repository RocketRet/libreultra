#include <os_internal.h>
#include "siint.h"
#include <assert.h>

// This whitespace is necessary for matching the assert















































s32 __osSiRawStartDma(s32 direction, void *dramAddr)
{
    assert(((u32)dramAddr & 3) == 0);
    if (__osSiDeviceBusy())
        return -1;

    if (direction == OS_WRITE)
        osWritebackDCache(dramAddr, 64);

    IO_WRITE(SI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));

    if (direction == OS_READ)
        IO_WRITE(SI_PIF_ADDR_RD64B_REG, 0x1FC007C0);
    else
        IO_WRITE(SI_PIF_ADDR_WR64B_REG, 0x1FC007C0);

    if (direction == OS_READ)
        osInvalDCache(dramAddr, 64);

    return 0;
}
