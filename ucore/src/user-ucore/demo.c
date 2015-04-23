#include <ulib.h>
#include <stdio.h>
#include <string.h>
#include <stat.h>
#include <unistd.h>
#include <file.h>

#define CHR_DEV_NAME "chr_dev:"

#define printf(...)                 fprintf(1, __VA_ARGS__)

int main() {
  int ret;
  char buf[32];
  printf("Before Open\n");
  int fd = open(CHR_DEV_NAME, O_RDONLY);
  printf("After Open\n");
  if (fd < 0) {
    printf("open file %s failed!\n", CHR_DEV_NAME);
    return -2012;
  }
  printf("Before Read\n");
  read(fd, buf, 32);
  printf("Before Close\n");
  close(fd);
  printf("After Close\n");
  return 0;
}
