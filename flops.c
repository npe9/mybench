#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int
main(int argc, char *argv[])
{
  struct timeval start, finish;
  int i,n;
  double *d,*a,*b,*c;
  double dd,aa, bb,cc;
  int secs;

  if(argc != 2) {
    fprintf(stderr, "usage: flops iters");
    exit(1);
  }
  n = atoi(argv[1]);
  d = malloc(n*sizeof(double));
  a = malloc(n*sizeof(double));
  b = malloc(n*sizeof(double));
  c = malloc(n*sizeof(double));
  gettimeofday(&start, NULL);
  for(i=0;i<n;i++){
    //_mm256_add_ps(_mm256_mul_ps(d[i],a[i]), c[i]);
    
    d[i] = a[i]*b[i]+c[i];
    dd = d[i];
    aa = a[i];
    bb = b[i];
    cc = c[i];
    dd =  aa*bb+cc;
  }
  gettimeofday(&finish, NULL);
  secs = (finish.tv_sec-start.tv_sec)*1000000+(finish.tv_usec-start.tv_usec);
  printf("elapsed %F flops %d\n", (finish.tv_sec-start.tv_sec)*1000000+(finish.tv_usec-start.tv_usec), n/secs);

}
