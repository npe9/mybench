#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static inline uint64_t rdtscp( uint32_t *aux )
{
  uint64_t rax,rdx;
  asm volatile ( "rdtscp\n" : "=a" (rax), "=d" (rdx), "=c" (*aux) : : );
  return (rdx << 32) + rax;

}
void
main(int argc, char *argv[])
{
  int pgsz;
  char *rgn;
  int i, n;
  unsigned long long elapsed;
  uint64_t start, finished;
  uint32_t aux;

  if(argc != 2){
    fprintf(stderr, "usage: mfault npages\n");
    exit(1);
  }
  n = strtoull(argv[1], NULL, 10);
  pgsz = getpagesize();
  /*
  rgn = mmap(NULL, pgsz*n, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
  madvise(rgn, pgsz*n, MADV_RANDOM);
  if(rgn == MAP_FAILED){
    fprintf(stderr, "couldn't map region\n");
    exit(1);
  }
  */
  rgn = malloc(pgsz*n);
  if(rgn == NULL){
    fprintf(stderr, "couldn't allocate region\n");
    exit(1);
  }
  if(fork() == 0)
    exit(0);
  start = rdtscp(&aux);
  rgn[0]=1;
  for(i = n; i; i--){
    rgn[i*pgsz] = rgn[(i-1)*pgsz];
  }
  finished = rdtscp(&aux);
  elapsed = finished - start;
  printf("cycles/fault %F\n", (double)elapsed/(double)n);
  //for(;;);
  //elapsed = (finished.tv_sec-start.tv_sec)*1000000+(finished.tv_usec-start.tv_usec);
  //printf("cycles/fault %F\n", (double)elapsed*2000/(double)n);

}
