#include <ultra64.h>

u64 __udivmoddi4(u64, u64, u64*);

// TODO regalloc
s64 __divdi3(s64 a, s64 b)
{
    u32 s2 = 0;
    s64 ret;

    if (a < 0)
    {
        s2 = ~s2;
        a = -a;
    }
    if (b < 0)
    {
        s2 = ~s2;
        b = -b;
    }
    ret = __udivmoddi4(a, b, NULL);
    if (s2)
    {
        ret = -ret;
    }
    return ret;
}

// INCLUDE_ASM(s32, "lib/codeseg1/codeseg1_228", __divdi3);
