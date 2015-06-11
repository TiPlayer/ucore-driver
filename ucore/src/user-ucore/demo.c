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
  char hello[] = "Hello, world!\n";
  char buffer[100];
  printf("Before Open\n");
  int fd = open(CHR_DEV_NAME, O_RDWR);
  printf("After Open, fd = %d\n", fd);
  if (fd < 0) {
    printf("open file %s failed!\n", CHR_DEV_NAME);
    return -2012;
  }
  printf("Before write\n");
  write(fd, hello, 15);
  printf("Before read\n");
  read(fd, buffer, 32);
  printf("Before Close\n");
  close(fd);
  printf("After Close\n");
  return 0;
}
