#include <linux/kconfig.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/printk.h>
#include <linux/uaccess.h>

static struct cdev chr_dev; //An instance of a character device
static dev_t ndev; //The node of the device

static uint8_t demo_buffer[256];

static int chr_open(struct inode *nd, struct file *filp) {
	int major = MAJOR(nd->i_rdev);
	int minor = MINOR(nd->i_rdev);
	printk("chr_open, major=%d, minor=%d\n", major, minor);
	return 0;
}

static ssize_t chr_read(struct file *filp, char __user *u, size_t sz, loff_t *off) {
	printk("In the chr_read() function!\n");
	return 0;
}

static ssize_t chr_write(struct file *filp, const char __user *u, size_t sz, loff_t *off) {
  printk("In the chr_write() function!\n");
  printk("u: %p, sz: %d, off:%ld\n", u, sz, *off);
  int result;
  loff_t pos = *off; //pos: offset

  if (pos >= 256) {
    result = 0;
    goto out;
  }
  if (sz > (256 - pos))
    sz = 256 - pos;
  pos += sz;

  if (copy_from_user(demo_buffer + *off, u, sz)) {
    sz = -EFAULT;
    goto out;
  }
  printk("demo: %s", demo_buffer + *off);
  *off = pos;
out:
  return sz;
}

static int chr_release(struct inode *nd, struct file *filp) {
	printk("In the chr_release() function!\n");
	return 0;       
}

// file_operations is a very important data struct in character type device
struct file_operations chr_ops = {
    .owner = THIS_MODULE,
    .open = chr_open,
    .read = chr_read,
    .write = chr_write,
    .release = chr_release,
};
// initialization function of the module
static int demo_init(void) {
	int ret;
	cdev_init(&chr_dev, &chr_ops); //initialize the device instance
  ret = alloc_chrdev_region(&ndev, 0, 1, "chr_dev"); //allocate the device node number dynamically
	if(ret < 0) return ret;
  printk("demo_init():major=%d, minor=%d\n", MAJOR(ndev), MINOR(ndev));
  ret = cdev_add(&chr_dev, ndev, 1);//register the char_dev into the system
	if(ret < 0) return ret;
	return 0;
}

static void demo_exit(void) {
	printk("demo_exit...\n");
	cdev_del(&chr_dev); //unregister the char_dev from the system
	unregister_chrdev_region(ndev, 1); //free the device node number
}

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
    .name = "mod-demo",
    .init = demo_init,
    .exit = demo_exit,
};


module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
