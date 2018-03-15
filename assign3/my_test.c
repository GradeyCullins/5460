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
  int i;
  int fd;
  int sleep_len;
  ssize_t r;
  
  fd = open("/dev/sleepy0", O_RDWR);
  assert(fd != -1);

  sleep_len = 10;
  r = write(fd, &sleep_len, sizeof sleep_len);
  assert(r >= 0);
  close(fd);

  


  /* read from device 9*/
//  fd = open("/dev/sleepy9", O_RDWR);
//  assert(fd != -1);
//  r = read(fd, NULL, 0);
//  assert(r >= 0);
//  close(fd);
//
//  sleep(7); /* sleep for 7 seconds*/
//
//  /* now read from device 0*/
//  fd = open("/dev/sleepy0", O_RDWR);
//  assert(fd != -1);
//  r = read(fd, NULL, 0);
//  assert(r >= 0);
//  close(fd);
  
  return 0;
}
