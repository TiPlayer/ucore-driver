#ifndef __ASM_STRING_64_H__
#define __ASM_STRING_64_H__

#define __HAVE_ARCH_MEMCPY 1

extern void *memcpy(void *to, const void *from, size_t len);

#define __HAVE_ARCH_MEMSET
void *memset(void *s, char c, size_t n);

#define __HAVE_ARCH_MEMMOVE

int memcmp(const void *cs, const void *ct, size_t count);
size_t strlen(const char *s);

#endif /* ! __ASM_STRING_64_H__ */
