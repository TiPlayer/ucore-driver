#ifndef __LINUX_EXPORT_H__
#define __LINUX_EXPORT_H__

struct kernel_symbol
{
	unsigned long value;
	const char *name;
};

#endif /* ! __LINUX_EXPORT_H__ */
