#ifndef __LINUX_PREEMPT_H__
#define __LINUX_PREEMPT_H__

#include <linux/thread_info.h>
#include <linux/linkage.h>
#include <linux/list.h>

/*
 * Even if we don't have any preemption, we need preempt disable/enable
 * to be barriers, so that we don't have things like get_user/put_user
 * that can cause faults and scheduling migrate into our preempt-protected
 * region.
 */
#define preempt_disable()		barrier()

#define preempt_enable_no_resched()	barrier()
#define preempt_enable()		barrier()

#define preempt_disable_notrace()		barrier()

#define preempt_enable_notrace()		barrier()

#endif /* ! __LINUX_PREEMPT_H__ */
