#include <linux/kconfig.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/printk.h>
#define __NO_UCORE_DEVICE__
#define __NO_UCORE_TYPE__
#include <dev.h>

#define CHRDEV_MAJOR_HASH_SIZE 256
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static struct char_device_struct {
  struct char_device_struct *next;
  unsigned int major;
  unsigned int baseminor;
  int minorct;
  char name[64];
  struct cdev *cdev;	/* will die */
} *chrdevs[CHRDEV_MAJOR_HASH_SIZE];

extern void *kmalloc(size_t size);
extern void kfree(void *objp);
extern size_t strlcpy(char *dest, const char *src, size_t size);
/* index in the above - for now: assume no multimajor ranges */
static inline int major_to_index(int major)  {
  return major % CHRDEV_MAJOR_HASH_SIZE;
}

struct char_device_struct* find_char_device(unsigned dev) {
  unsigned major = MAJOR(dev);
  unsigned minor = MINOR(dev);
  struct char_device_struct* p = chrdevs[major];
  while (p && p->baseminor != minor) {
    p = p->next;
  }
  if (!p) {
    return NULL;
  } else {
    return p;
  }
}

static struct char_device_struct *
__register_chrdev_region(unsigned int major, unsigned int baseminor, int minorct, const char *name) {
  struct char_device_struct *cd, **cp;
  int ret = 0;
  int i;
  
  cd = kmalloc(sizeof(struct char_device_struct));
  if (cd == NULL)
    return ERR_PTR(-ENOMEM);
  
  //mutex_lock(&chrdevs_lock);
  
  /* temporary */
  if (major == 0) {
    for (i = ARRAY_SIZE(chrdevs) - 1; i > 0; i--) {
      if (chrdevs[i] == NULL)
        break;
    }
    
    if (i == 0) {
      ret = -EBUSY;
      goto out;
    }
    major = i;
    ret = major;
  }
  
  cd->major = major;
  cd->baseminor = baseminor;
  cd->minorct = minorct;
  strlcpy(cd->name, name, sizeof(cd->name));
  
  i = major_to_index(major);
  
  for (cp = &chrdevs[i]; *cp; cp = &(*cp)->next)
    if ((*cp)->major > major ||
        ((*cp)->major == major &&
         (((*cp)->baseminor >= baseminor) ||
          ((*cp)->baseminor + (*cp)->minorct > baseminor))))
      break;
  
  /* Check for overlapping minor ranges.  */
  if (*cp && (*cp)->major == major) {
    int old_min = (*cp)->baseminor;
    int old_max = (*cp)->baseminor + (*cp)->minorct - 1;
    int new_min = baseminor;
    int new_max = baseminor + minorct - 1;
    
    /* New driver overlaps from the left.  */
    if (new_max >= old_min && new_max <= old_max) {
      ret = -EBUSY;
      goto out;
    }
    
    /* New driver overlaps from the right.  */
    if (new_min <= old_max && new_min >= old_min) {
      ret = -EBUSY;
      goto out;
    }
  }
  
  cd->next = *cp;
  *cp = cd;
//  mutex_unlock(&chrdevs_lock);
  return cd;
out:
//  mutex_unlock(&chrdevs_lock);
  kfree(cd);
  return ERR_PTR(ret);
}


int alloc_chrdev_region(dev_t * dev, unsigned int baseminor, unsigned int count, const char * name) {
	struct char_device_struct *cd;
  cd = __register_chrdev_region(0, baseminor, count, name);
  if (IS_ERR(cd))
    return PTR_ERR(cd);
  *dev = MKDEV(cd->major, cd->baseminor);
  return 0;
}

struct ucore_iobuf {
  void *io_base;		/* The base addr of object       */
  loff_t io_offset;	/* Desired offset into object    */
  size_t io_len;		/* The lenght of Data            */
  size_t io_resid;	/* Remaining amt of data to xfer */
};

int ucore_cdev_open_adapter(struct ucore_device * dev, uint32_t open_flags) {
  struct inode node;
  node.i_rdev = dev->i_rdev;
  return dev->i_fops->open(&node, NULL);
}
int ucore_cdev_close_adapter (struct ucore_device * dev) {
  return dev->i_fops->release(NULL, NULL);
}
int ucore_cdev_io_adapter (struct ucore_device * dev, struct ucore_iobuf * iob, bool write) {
  printk("io_offset = %p\n", &(iob->io_offset));
  if (write) {
    return (int)dev->i_fops->write(NULL, iob->io_base, iob->io_len, &(iob->io_offset));
  } else {
    return (int)dev->i_fops->read(NULL, iob->io_base, iob->io_len, &(iob->io_offset));
  }
}
int ucore_cdev_ioctl_adapter (struct ucore_device * dev, int op, void *data) {
  printk("ucore_cdev_ioctl_adapter not implemented\n");
  return 0;
}

extern void addLinuxDevice(struct ucore_device* udev);

int cdev_add(struct cdev * cdev, dev_t dev, unsigned int count) {
  struct char_device_struct* char_device_pointer = find_char_device(dev);
  if (!char_device_pointer) {
    printk("char_device: %x not find!\n", dev);
  }
  cdev->dev = dev;
  cdev->count = count;
  struct ucore_device udev;
  strcpy(udev.d_name, char_device_pointer->name);
  udev.d_blocks = 0;
  udev.d_blocksize = 1;
  udev.i_rdev = cdev->dev;
  udev.count = cdev->count;
  udev.i_fops = cdev->ops;
  udev.d_open = ucore_cdev_open_adapter;
  udev.d_close = ucore_cdev_close_adapter;
  udev.d_io = ucore_cdev_io_adapter;
  udev.d_close = ucore_cdev_close_adapter;
  addLinuxDevice(&udev);
	return 0;
}

/*
 */
void cdev_del(struct cdev * a) {
	printk("cdev_del not implemented\n");
}

/*
 */
void cdev_init(struct cdev * cdev, const struct file_operations * fops) {
	memset(cdev, 0, sizeof *cdev);
  INIT_LIST_HEAD(&cdev->list);
  // kobject_init(&cdev->kobj, &ktype_cdev_default);
  cdev->ops = fops;
}

static struct char_device_struct *
__unregister_chrdev_region(unsigned major, unsigned baseminor, int minorct) {
  struct char_device_struct *cd = NULL, **cp;
  int i = major_to_index(major);
//  mutex_lock(&chrdevs_lock);
  for (cp = &chrdevs[i]; *cp; cp = &(*cp)->next)
    if ((*cp)->major == major &&
        (*cp)->baseminor == baseminor && (*cp)->minorct == minorct)
      break;
  if (*cp) {
    cd = *cp;
    *cp = cd->next;
  }
//  mutex_unlock(&chrdevs_lock);
  return cd;
}

void unregister_chrdev_region(dev_t from, unsigned int count) {
  dev_t to = from + count;
  dev_t n, next;
  for (n = from; n < to; n = next) {
    next = MKDEV(MAJOR(n) + 1, 0);
    if (next > to)
      next = to;
    kfree(__unregister_chrdev_region(MAJOR(n), MINOR(n), next - n));
  }
}

#ifndef MODULE_SYMBOL_PREFIX
#define MODULE_SYMBOL_PREFIX ""
#endif

#define __EXPORT_SYMBOL(sym, sec) 				\
	extern typeof(sym) sym; 			\
	static const char __kstrtab_##sym[] \
	__attribute__((section("__ksymtab_strings"), aligned(1))) \
	= MODULE_SYMBOL_PREFIX #sym; 		\
	static const struct kernel_symbol __ksymtab_##sym \
	__used								\
	__attribute__((section("__ksymtab" sec), unused)) \
	= { (unsigned long)&sym, __kstrtab_##sym }

#define EXPORT_SYMBOL(sym) __EXPORT_SYMBOL(sym, "")

EXPORT_SYMBOL(alloc_chrdev_region);
EXPORT_SYMBOL(cdev_add);
EXPORT_SYMBOL(cdev_del);
EXPORT_SYMBOL(cdev_init);
EXPORT_SYMBOL(unregister_chrdev_region);

