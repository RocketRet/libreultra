#include <bss.h>

#include <os_internal.h>

#define PI_Q_BUF_LEN 1
u32 __osPiAccessQueueEnabled = 0;
static OSMesg piAccessBuf[PI_Q_BUF_LEN] BSS;
static u8 pad1[0xC] BSS;
OSMesgQueue __osPiAccessQueue BSS;

void __osPiCreateAccessQueue(void)
{

	__osPiAccessQueueEnabled = 1;
	osCreateMesgQueue(&__osPiAccessQueue, piAccessBuf, PI_Q_BUF_LEN);
	osSendMesg(&__osPiAccessQueue, NULL, OS_MESG_NOBLOCK);
}
void __osPiGetAccess(void)
{
	OSMesg dummyMesg;
	if (!__osPiAccessQueueEnabled)
		__osPiCreateAccessQueue();
	osRecvMesg(&__osPiAccessQueue, &dummyMesg, OS_MESG_BLOCK);
}
void __osPiRelAccess(void)
{
	osSendMesg(&__osPiAccessQueue, NULL, OS_MESG_NOBLOCK);
}
