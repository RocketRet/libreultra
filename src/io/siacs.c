#include <bss.h>
#include <os_internal.h>

#define SI_Q_BUF_LEN 1
OSMesg siAccessBuf[SI_Q_BUF_LEN] BSS;
static u8 pad1[0xC] BSS;
OSMesgQueue __osSiAccessQueue BSS;
u32 __osSiAccessQueueEnabled = 0;

void __osSiCreateAccessQueue(void)
{
	__osSiAccessQueueEnabled = 1;
	osCreateMesgQueue(&__osSiAccessQueue, siAccessBuf, SI_Q_BUF_LEN);
	osSendMesg(&__osSiAccessQueue, NULL, OS_MESG_NOBLOCK);
}
void __osSiGetAccess(void)
{
	OSMesg dummyMesg;
	if (!__osSiAccessQueueEnabled)
		__osSiCreateAccessQueue();
	osRecvMesg(&__osSiAccessQueue, &dummyMesg, OS_MESG_BLOCK);
}
void __osSiRelAccess(void)
{
	osSendMesg(&__osSiAccessQueue, NULL, OS_MESG_NOBLOCK);
}
