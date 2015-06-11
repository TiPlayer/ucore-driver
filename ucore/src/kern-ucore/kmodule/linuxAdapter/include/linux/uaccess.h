extern unsigned long __ucore_copy_from_user(void *to, const void __user *from, unsigned long n);
extern unsigned long __ucore_copy_to_user(void __user *to, const void *from, unsigned long n);

static inline unsigned long copy_from_user(void *to, const void __user *from, unsigned long n) {
  return __ucore_copy_from_user(to, from, n);
}

static inline unsigned long copy_to_user(void __user *to, const void *from, unsigned long n) {
  return __ucore_copy_to_user(to, from, n);
}
