#ifndef __LINUX_BUG_H__
#define __LINUX_BUG_H__

#include <asm/bug.h>
#include <linux/compiler.h>

struct pt_regs;

/*
 * BUILD_BUG_ON_INVALID() permits the compiler to check the validity of the
 * expression but avoids the generation of any code, even if that expression
 * has side-effects.
 */
#define BUILD_BUG_ON_INVALID(e) ((void)(sizeof((__force long)(e))))

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#include <asm-generic/bug.h>

#endif /* ! __LINUX_BUG_H__ */
