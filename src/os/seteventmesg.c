#include <os_internal.h>
#include "osint.h"

__OSEventState __osEventStateTab[OS_NUM_EVENTS] __attribute__((aligned(8))) __attribute__ ((section (".bss")));

void osSetEventMesg(OSEvent event, OSMesgQueue *mq, OSMesg msg)
{
	register u32 saveMask = __osDisableInt();
	__OSEventState *es;

	es = &__osEventStateTab[event];
	es->messageQueue = mq;
	es->message = msg;
	__osRestoreInt(saveMask);
}
