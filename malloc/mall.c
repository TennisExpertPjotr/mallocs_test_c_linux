#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STEP (1024ULL*1024)
#define LIMIT (1024ULL*1024*128)
#define ull unsigned long long

long long get_time()
{
    struct timespec tstart={0,0};
    clock_gettime(CLOCK_MONOTONIC,&tstart);
    return tstart.tv_sec*1000000000LL+tstart.tv_nsec;
}

int main()
{
  printf("| %-10s | %-14s | %-14s |\n", "SIZE kb", "TIME MALLOC ns", "TIME FREE ns");
  printf("|------------|----------------|----------------|\n");
  long long avg_mall, avg_free;
  for(ull size = 1024; size <= LIMIT; size += STEP)
  {
    avg_mall = 0;
    avg_free = 0;
    for(int i=0; i<1000; i++)
      {
        long long t1=get_time();
        char * buf =(char*)malloc(size);
        long long t2=get_time();
        free(buf);
        long long t3=get_time();
        avg_mall += (t2-t1);
        avg_free += (t3-t2);
      }
    printf("| %-10llu | %-14lld | %-14lld |\n", size/1024, avg_mall/1000, avg_free/1000);
  }
  return 0;
}
