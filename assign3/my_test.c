/** program to test the kernel module **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/wait.h>

int main(void) {  
  int fd;
  
  fd = open("/dev/sleepy0", O_RDWR);
  assert(fd != -1);
  
  return 0;
}
