#ifndef __LINUX_RWSEM_H__
#define __LINUX_RWSEM_H__

#define _LINUX_RWSEM_H

#include <linux/linkage.h>

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/spinlock.h>

#include <linux/atomic.h>

struct rw_semaphore;

struct rw_semaphore {
	long			count;
	raw_spinlock_t		wait_lock;
	struct list_head	wait_list;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
};

#include <asm/rwsem.h>

#endif /* ! __LINUX_RWSEM_H__ */
