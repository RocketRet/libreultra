#include <bss.h>

#include <os_internal.h>
#include "osint.h"

// See below for an explanation of how this is a bugfix
#ifndef BUGFIXES
u8 __osBaseTimerBug[];
OSTimer *__osTimerList = (OSTimer*)&__osBaseTimerBug;
#else
OSTimer *__osTimerList = &__osBaseTimer;
#endif

// OSTimer __osBaseTimer;
// OSTime __osCurrentTime;
// u32 __osBaseCounter;
// u32 __osViIntrCount;
// u32 __osTimerCounter;

OSTime __osCurrentTime BSS;
static u8 pad1[0x8] BSS;
u32 __osViIntrCount BSS;
static u8 pad2[0xC] BSS;
u32 __osTimerCounter BSS;
static u8 pad3[0xC] BSS;
u32 __osBaseCounter BSS;
static u8 pad4[0xC + 0x8] BSS;

//! BUG: This should be OSTimer __osBaseTimer, but due to a bug in the SN64 toolchain
// it overlaps the memory occupied by `siAccessBuf` in siacs.c. Therefore, this needs
// to be declared as a smaller variable so that the two can be linked, which results
// in an out-of-bounds access in `__osTimerServicesInit`.
#ifndef BUGFIXES
u8 __osBaseTimerBug[0x10] BSS;
#else
OSTimer __osBaseTimer BSS;
#endif

void __osTimerServicesInit(void)
{
	__osCurrentTime = 0;
	__osBaseCounter = 0;
	__osViIntrCount = 0;
	__osTimerList->prev = __osTimerList;
	__osTimerList->next = __osTimerList->prev;
	__osTimerList->value = 0;
	__osTimerList->interval = __osTimerList->value;
	__osTimerList->mq = NULL;
	__osTimerList->msg = 0;
}

void __osTimerInterrupt(void)
{
	OSTimer *t;
	u32 count;
	u32 elapsed_cycles;

	if (__osTimerList->next == __osTimerList)
		return;
	while (1)
	{
		t = __osTimerList->next;
		if (t == __osTimerList)
		{
			__osSetCompare(0);
			__osTimerCounter = 0;
			break;
		}
		count = osGetCount();
		elapsed_cycles = count - __osTimerCounter;
		__osTimerCounter = count;
		if (elapsed_cycles < t->value)
		{
			t->value -= elapsed_cycles;
			__osSetTimerIntr(t->value);
			return;
		}
		else
		{
			t->prev->next = t->next;
			t->next->prev = t->prev;
			t->next = NULL;
			t->prev = NULL;
			if (t->mq != NULL)
			{
				osSendMesg(t->mq, t->msg, OS_MESG_NOBLOCK);
			}
			if (t->interval != 0)
			{
				t->value = t->interval;
				__osInsertTimer(t);
			}
		}
	}
}
//
void __osSetTimerIntr(OSTime tim)
{
	OSTime NewTime;
	u32 savedMask;
	savedMask = __osDisableInt();
	__osTimerCounter = osGetCount();
	NewTime = __osTimerCounter + tim;
	__osSetCompare(NewTime);
	__osRestoreInt(savedMask);
}
OSTime __osInsertTimer(OSTimer *t)
{
	OSTimer *timep;
	OSTime tim;
	u32 savedMask;
	savedMask = __osDisableInt();
	for (timep = __osTimerList->next, tim = t->value;
		 timep != __osTimerList && tim > timep->value;
		 tim -= timep->value, timep = timep->next)
	{
		;
	}
	t->value = tim;
	if (timep != __osTimerList)
		timep->value -= tim;
	t->next = timep;
	t->prev = timep->prev;
	timep->prev->next = t;
	timep->prev = t;
	__osRestoreInt(savedMask);
	return tim;
}
