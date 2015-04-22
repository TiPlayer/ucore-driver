#ifndef __LINUX_PRINTK_H__
#define __LINUX_PRINTK_H__

#include <linux/init.h>
#include <linux/kern_levels.h>
#include <linux/linkage.h>

static inline int printk_get_level(const char *buffer)
{
	if (buffer[0] == KERN_SOH_ASCII && buffer[1]) {
		switch (buffer[1]) {
		case '0' ... '7':
		case 'd':	/* KERN_DEFAULT */
			return buffer[1];
		}
	}
	return 0;
}

extern int console_printk[];

#define console_loglevel (console_printk[0])

asmlinkage __printf(1, 2) __cold
int printk(const char *fmt, ...);

extern asmlinkage void dump_stack(void) __cold;

#endif /* ! __LINUX_PRINTK_H__ */
