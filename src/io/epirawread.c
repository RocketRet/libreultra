#include <os_internal.h>
#include <rcp.h>
#include "piint.h"
#include <assert.h>

// This whitespace is necessary for matching the assert































s32 osEPiRawReadIo(OSPiHandle *pihandle, u32 devAddr, u32 *data)
{
    register u32 stat;
    register u32 domain;

    assert_filename(data != NULL, "epirawread.c");

    EPI_SYNC(pihandle,stat,domain);
    
    *data = IO_READ(pihandle->baseAddress | devAddr);
    return 0;
}
