#include <linux/kconfig.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/printk.h>


#define DDE_WEAK __attribute__((weak))

#define dde_dummy_printf(...)
#define dde_printf(...) dde_dummy_printf(__VA_ARGS__)

/*
 */
DDE_WEAK int alloc_chrdev_region(dev_t * a, unsigned int b, unsigned int c, const char * d) {
	dde_printf("alloc_chrdev_region not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int cdev_add(struct cdev * a, dev_t b, unsigned int c) {
	dde_printf("cdev_add not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void cdev_del(struct cdev * a) {
	dde_printf("cdev_del not implemented\n");
}

/*
 */
DDE_WEAK void cdev_init(struct cdev * a, const struct file_operations * b) {
	dde_printf("cdev_init not implemented\n");
}

/*
 */
DDE_WEAK int printk(const char * a, ...) {
	dde_printf("printk not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void unregister_chrdev_region(dev_t a, unsigned int b) {
	dde_printf("unregister_chrdev_region not implemented\n");
}

