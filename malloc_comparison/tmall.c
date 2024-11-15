#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h> // Для использования dlsym
#include "my_jemalloc.h"

#define STEP (1024ULL*1024)
#define LIMIT (1024ULL*1024*128)
#define ull unsigned long long

// Параметры для динамической загрузки функций
void *(*tcmalloc)(size_t) = NULL;
void (*tcfree)(void *) = NULL;

long long get_time() {
    struct timespec tstart = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    return tstart.tv_sec * 1000000000LL + tstart.tv_nsec;
}

void init_tcmalloc() {
    static int initialized = 0;
    if (!initialized) {
        void *handle = dlopen("libtcmalloc_minimal.so", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Could not open tcmalloc library: %s\n", dlerror());
            exit(1);
        }

        tcmalloc = dlsym(handle, "tc_malloc");
        tcfree = dlsym(handle, "tc_free");
        if (!tcmalloc || !tcfree) {
            fprintf(stderr, "Could not find tcmalloc functions: %s\n", dlerror());
            exit(1);
        }
        initialized = 1;
    }
}


int main() {
    printf("| %-8s | %-9s | %-9s | %-11s | %-11s | %-10s | %-10s |\n", 
    "SIZE kb", "TIME M ns", "TIME F ns", "TIME TCM ns", "TIME TSF ns", "TIME JM ns", "TIME JF ns");
    printf("|----------|-----------|-----------|-------------|-------------|------------|------------|\n");

    long long avg_mall, avg_free, avg_tcmall, avg_tcfree, avg_jemall, avg_jefree;

    // Инициализация tcmalloc перед началом
    init_tcmalloc();

    for (ull size = STEP; size <= LIMIT; size += STEP) {
        avg_mall = 0;
        avg_free = 0;
        avg_tcmall = 0;
        avg_tcfree = 0;

        for (int i = 0; i < 1000; i++) {
            long long t1 = get_time();
            char *buf = (char *)malloc(size); 
            long long t2 = get_time();
            free(buf); 
            long long t3 = get_time();
            
            buf = (char *)tcmalloc(size); // Используйте tcmalloc
            long long t4 = get_time();
            tcfree(buf); // Используйте tcfree
            long long t5 = get_time();
            
            buf = (char *)jemalloc(size); // jemalloc
            long long t6 = get_time();
            jefree(buf); // jefree
            long long t7 = get_time();

            avg_mall += (t2 - t1);
            avg_free += (t3 - t2);
            avg_tcmall += (t4 - t3);
            avg_tcfree += (t5 - t4);
            avg_jemall += (t6 - t5);
            avg_jefree += (t7 - t6);
        }

        printf("| %-8llu | %-9lld | %-9lld | %-11lld | %-11lld | %-10lld | %-10lld |\n", 
        size/1024, avg_mall/1000, avg_free/1000, avg_tcmall/1000, avg_tcfree/1000, avg_jemall / 1000, avg_jefree / 1000);
    }

    return 0;
}
