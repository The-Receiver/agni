#include <cpuid_kern.h>

void cpuid_get_name(char *buf)
{
	uint32_t *brand = (uint32_t *) buf;
	__cpuid(0x80000002, brand[0], brand[1], brand[2], brand[3]);
	__cpuid(0x80000003, brand[4], brand[5], brand[6], brand[7]);
	__cpuid(0x80000004, brand[8], brand[9], brand[10], brand[11]);
}
