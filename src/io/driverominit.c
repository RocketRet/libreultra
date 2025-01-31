#include <bss.h>
#include <os_internal.h>
#include <rcp.h>
#include <bstring.h>

OSPiHandle DriveRomHandle BSS;
OSPiHandle *osDriveRomInit()
{
	u32 saveMask;
	if (DriveRomHandle.baseAddress == PHYS_TO_K1(PI_DOM1_ADDR1))
	{
		return &DriveRomHandle;
	}
	DriveRomHandle.type = DEVICE_TYPE_BULK;
	DriveRomHandle.baseAddress = PHYS_TO_K1(PI_DOM1_ADDR1);
	DriveRomHandle.latency = 64; //TODO: remove magic constant
	DriveRomHandle.pulse = 7;
	DriveRomHandle.pageSize = 7;
	DriveRomHandle.relDuration = 2;
	DriveRomHandle.domain = PI_DOMAIN1;
	DriveRomHandle.speed = 0;

	bzero(&DriveRomHandle.transferInfo, sizeof(__OSTranxInfo));

	saveMask = __osDisableInt();
	DriveRomHandle.next = __osPiTable;
	__osPiTable = &DriveRomHandle;
	__osRestoreInt(saveMask);

	return &DriveRomHandle;
}
