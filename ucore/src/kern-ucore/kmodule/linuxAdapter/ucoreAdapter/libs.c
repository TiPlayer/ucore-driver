#include "mod.h"
#include <types.h>
#include <kio.h>
#include <wait.h>
#include <sync.h>
#include <proc.h>
#include <sched.h>
#include <dev.h>
#include <vfs.h>
#include <iobuf.h>
#include <inode.h>
#include <unistd.h>
#include <error.h>
#include <assert.h>
#include <mod.h>

int printk(const char * fmt, ...) {
	va_list ap;
	int cnt;
	va_start(ap, fmt);
	cnt = vkprintf(fmt, ap);
	va_end(ap);
	return cnt;
}
EXPORT_SYMBOL(printk);

#ifndef __HAVE_ARCH_STRLCPY
size_t strlcpy(char *dest, const char *src, size_t size) {
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


void addLinuxDevice(struct device* dev) {
  struct inode *node;
  if ((node = dev_create_inode()) == NULL) {
    panic("stdin: dev_create_node.\n");
  }
  memcpy(&(node->in_info.__device_info), dev, sizeof(struct device));
  int ret;
  if ((ret = vfs_add_dev(node->in_info.__device_info.d_name, node, 0)) != 0) {
    panic("%s: vfs_add_dev: %e.\n", node->in_info.__device_info.d_name, ret);
  }
}
EXPORT_SYMBOL(addLinuxDevice);
