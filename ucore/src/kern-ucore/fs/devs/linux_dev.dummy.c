#include <linux/kconfig.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/err.h>

#define __NO_UCORE_TYPE__
#include <module.h>
#include <kio.h>



#ifdef __DEBUG
#define _TODO_() printk(KERN_ALERT "TODO %s\n", __func__)
#else
#define _TODO_()
#endif




struct blocking_notifier_head;
struct notifier_block;
struct attribute_group;
struct bin_attribute;

/* mutux */
void __mutex_init(struct mutex *lock, const char *name,
    struct lock_class_key *key)
{
}

extern void __init_rwsem(struct rw_semaphore *sem, const char *name,
    struct lock_class_key *key)
{
}

void mutex_lock(struct mutex *lock)
{
  //TODO
}


#ifndef __HAVE_ARCH_STRLCPY
/**
* strlcpy - Copy a %NUL terminated string into a sized buffer
* @dest: Where to copy the string to
* @src: Where to copy the string from
* @size: size of destination buffer
*
* Compatible with *BSD: the result is always a valid
* NUL-terminated string that fits in the buffer (unless,
* of course, the buffer size is zero). It does not pad
* out the result like strncpy() does.
*/
size_t strlcpy(char *dest, const char *src, size_t size)
{
  size_t ret = strlen(src);

  if (size) {
    size_t len = (ret >= size) ? size - 1 : ret;
    memcpy(dest, src, len);
    dest[len] = '\0';
  }
  return ret;
}

EXPORT_SYMBOL(strlcpy);
#endif


int mutex_lock_interruptible(struct mutex *lock)
{
  return 0;
}

int printk(const char * fmt, ...) {
  va_list ap;
  int cnt;
  va_start(ap, fmt);
  cnt = vkprintf(fmt, ap);
  va_end(ap);
  return cnt;
}


void mutex_unlock(struct mutex *lock)
{
  //TODO
}

void synchronize_rcu(void)
{

}

void *kzalloc(size_t size, gfp_t flags)  {
  return kmalloc(size, flags | __GFP_ZERO);
}

/* notifier */
int blocking_notifier_call_chain(struct blocking_notifier_head *nh,
    unsigned long val, void *v)
{
  return 0;
}

extern int blocking_notifier_chain_register(struct blocking_notifier_head *nh,
    struct notifier_block *nb)
{
  return 0;
}

extern int blocking_notifier_chain_unregister(struct blocking_notifier_head *nh,
    struct notifier_block *nb)
{
  return 0;
}

/* sysfs */
int sysfs_create_link(struct kobject *kobj, struct kobject *target,
    const char *name)
{
  _TODO_();
  return 0;
}

int sysfs_create_dir(struct kobject *kobj)
{
  _TODO_();
  return 0;
}

void sysfs_remove_dir(struct kobject *kobj)
{
  return;
}

int sysfs_rename_dir(struct kobject *kobj, const char *new_name)
{
  return 0;
}

int sysfs_move_dir(struct kobject *kobj, struct kobject *new_parent_kobj)
{
  return 0;
}

/**
*      sysfs_create_link_nowarn - create symlink between two objects.
*      @kobj:  object whose directory we're creating the link in.
*      @target:        object we're pointing to.
*      @name:          name of the symlink.
*
*      This function does the same as sysf_create_link(), but it
*      doesn't warn if the link already exists.
*/
int sysfs_create_link_nowarn(struct kobject *kobj, struct kobject *target,
    const char *name)
{
  return 0;
}

/**
*      sysfs_remove_link - remove symlink in object's directory.
*      @kobj:  object we're acting for.
*      @name:  name of the symlink to remove.
*/

void sysfs_remove_link(struct kobject *kobj, const char *name)
{
}

int sysfs_create_file(struct kobject *kobj, const struct attribute *attr)
{
  _TODO_();
  return 0;
}

int sysfs_chmod_file(struct kobject *kobj, struct attribute *attr, mode_t mode)
{
  return 0;
}

void sysfs_remove_file(struct kobject *kobj, const struct attribute *attr)
{
  return;
}

int sysfs_create_group(struct kobject *kobj, const struct attribute_group *grp)
{
  return 0;
}

int sysfs_update_group(struct kobject *kobj, const struct attribute_group *grp)
{
  return 0;
}

void sysfs_remove_group(struct kobject *kobj, const struct attribute_group *grp)
{
}

int sysfs_create_bin_file(struct kobject *kobj, struct bin_attribute *attr)
{
  return 0;
}

void sysfs_remove_bin_file(struct kobject *kobj, struct bin_attribute *attr)
{
  return;
}

int sysfs_schedule_callback(struct kobject *kobj, void (*func) (void *),
    void *data, struct module *owner)
{
  return 0;
}

int add_uevent_var(struct kobj_uevent_env *env, const char *format, ...)
{
  printk("INFO: add_uevent_var: %s\n", format);
  return 0;
}

int kobject_uevent(struct kobject *kobj, enum kobject_action action)
{
  return 0;
}

int kobject_uevent_env(struct kobject *kobj, enum kobject_action action,
    char *envp[])
{
  return 0;
}

int kobject_action_type(const char *buf, size_t count,
    enum kobject_action *type)
{
  return 0;
}

/* firmware */
int firmware_init(void)
{
  return 0;
}

/* cpu */
int cpu_dev_init()
{
  return 0;
}

/* dma */
int
dma_declare_coherent_memory(struct device *dev, dma_addr_t bus_addr,
    dma_addr_t device_addr, size_t size, int flags)
{
  return 0;
}

void dma_release_declared_memory(struct device *dev)
{
}

void *dma_mark_declared_memory_occupied(struct device *dev,
    dma_addr_t device_addr, size_t size)
{
  return ERR_PTR(-EBUSY);
}

void *dma_alloc_coherent(struct device *dev, size_t size,
    dma_addr_t * dma_handle, gfp_t flag)
{
  //        return pci_alloc_consistent(to_pci_dev(dev), size, dma_handle);
  panic("TODO");
  return NULL;
}

void
dma_free_coherent(struct device *dev, size_t size, void *cpu_addr,
    dma_addr_t dma_handle)
{
  printk(KERN_ALERT "TODO %s\n", __func__);
}

/* console */
void acquire_console_sem(void)
{
}

int try_acquire_console_sem(void)
{
  return 0;
}

void release_console_sem(void)
{
}

/* uaccess */

/* procfs */
struct proc_dir_entry *proc_create_data(const char *name, mode_t mode,
    struct proc_dir_entry *parent,
    const struct file_operations *proc_fops,
    void *data)
{
  //TODO
  printk(KERN_DEBUG "proc_create_data: %s\n", name);
  return NULL;
}

void remove_proc_entry(const char *name, struct proc_dir_entry *parent)
{

}

struct proc_dir_entry *proc_mkdir(const char *name, struct proc_dir_entry *p)
{
  return NULL;
}

int seq_open(struct file *file, const struct seq_operations *op)
{
  return -EINVAL;
}

ssize_t seq_read(struct file * file, char __user * buf, size_t size,
    loff_t * ppos)
{
  return -EINVAL;
}

loff_t seq_lseek(struct file * file, loff_t offset, int origin)
{
  return -EINVAL;
}

int seq_release(struct inode *inode, struct file *file)
{
  return -EINVAL;
}

int seq_escape(struct seq_file *m, const char *s, const char *esc)
{
  return -EINVAL;
}

int seq_putc(struct seq_file *m, char c)
{
  return -EINVAL;
}

int seq_puts(struct seq_file *m, const char *s)
{
  return -EINVAL;
}

int seq_write(struct seq_file *seq, const void *data, size_t len)
{
  return -EINVAL;
}

int seq_printf(struct seq_file *m, const char *f, ...)
{
  return -EINVAL;
}

struct list_head *seq_list_start(struct list_head *head, loff_t pos)
{
  return NULL;
}

struct list_head *seq_list_next(void *v, struct list_head *head, loff_t * ppos)
{
  return NULL;
}

#define VM_IO           0x00004000      /* Memory mapped I/O or similar */

/* vmm */
int remap_pfn_range(struct vm_area_struct *vma, unsigned long addr,
    unsigned long pfn, unsigned long size, pgprot_t prot)  {
  return 0;
}

/* bdi api, what is it? */
int bdi_init(struct backing_dev_info *bdi)
{
  return 0;
}
