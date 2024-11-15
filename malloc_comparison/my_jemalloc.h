#ifndef MY_JEMALLOC_H
#define MY_JEMALLOC_H

#include <jemalloc/jemalloc.h>


void* jemalloc(unsigned long long size) {
    return malloc(size);
}

void jefree(void* ptr) {
    return free(ptr);
}


#endif
