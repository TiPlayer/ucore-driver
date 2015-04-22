#ifndef __LINUX_MODULEPARAM_H__
#define __LINUX_MODULEPARAM_H__

#include <linux/init.h>
#include <linux/stringify.h>
#include <linux/kernel.h>

#define MAX_PARAM_PREFIX_LEN (64 - sizeof(unsigned long))

#define __MODULE_INFO(tag, name, info)					  \
  struct __UNIQUE_ID(name) {}

struct kernel_param;

struct kernel_param_ops {
	/* How the ops should behave */
	unsigned int flags;
	/* Returns 0, or -errno.  arg is in kp->arg. */
	int (*set)(const char *val, const struct kernel_param *kp);
	/* Returns length written or -errno.  Buffer is 4k (ie. be short!) */
	int (*get)(char *buffer, const struct kernel_param *kp);
	/* Optional function to free kp->arg when module unloaded. */
	void (*free)(void *arg);
};

struct kernel_param {
	const char *name;
	const struct kernel_param_ops *ops;
	u16 perm;
	s16 level;
	union {
		void *arg;
		const struct kparam_string *str;
		const struct kparam_array *arr;
	};
};

struct kparam_string {
	unsigned int maxlen;
	char *string;
};

struct kparam_array
{
	unsigned int max;
	unsigned int elemsize;
	unsigned int *num;
	const struct kernel_param_ops *ops;
	void *elem;
};

struct module;

#endif /* ! __LINUX_MODULEPARAM_H__ */
