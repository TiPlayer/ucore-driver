#ifndef __ASM_CURRENT_H__
#define __ASM_CURRENT_H__

#include <linux/compiler.h>
#include <asm/percpu.h>

struct task_struct;

DECLARE_PER_CPU(struct task_struct *, current_task);

#endif /* ! __ASM_CURRENT_H__ */
