#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

static unsigned char fromKernel[4];

int main() {
 int ret, file;

 file = open("/dev/random", O_RDWR);
 if (file < 0) {
  perror("\nERROR: file open()\n");
  return errno;
 }

 ret = read(file, fromKernel, 4);
 if (ret < 0) {
  perror("\nERROR: read()\n");
  return 0;
 }
 
 int result = 0;
 
 for (int i = 0; i < 4; ++i) {
  result += (fromKernel[i] - '0');
  result *= 10;
 }
 
 printf("Random number: %d\n", result);
 return 0;
}
