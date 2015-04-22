#ifndef __LINUX_RCUPDATE_H__
#define __LINUX_RCUPDATE_H__

#include <linux/types.h>
#include <linux/cache.h>
#include <linux/spinlock.h>
#include <linux/threads.h>
#include <linux/cpumask.h>
#include <linux/seqlock.h>
#include <linux/lockdep.h>
#include <linux/completion.h>
#include <linux/debugobjects.h>
#include <linux/bug.h>
#include <linux/compiler.h>

extern void synchronize_sched(void);

static inline void __rcu_read_lock(void)
{
	preempt_disable();
}

static inline void __rcu_read_unlock(void)
{
	preempt_enable();
}

#include <linux/rcutree.h>

# define rcu_lock_acquire(a)		do { } while (0)
# define rcu_lock_release(a)		do { } while (0)

#define rcu_lockdep_assert(c, s) do { } while (0)

#endif /* ! __LINUX_RCUPDATE_H__ */
