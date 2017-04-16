//
// Created by miroslav on 4/15/17.
//
#include "regions.h"

#include <stdlib.h>
#include <assert.h>

struct _Region {
    int size;
    size_t capacity;
    void** heapAllocatedStuff;
};

Region *NewRegion() {
    Region *r = malloc(sizeof(Region));
    r->size = 0;
    r->capacity = 64;
    r->heapAllocatedStuff = malloc(sizeof(void*) * r->capacity);
    return r;
}
char *RegionAlloc(Region *r, size_t size) {
    if(r->size == r->capacity) {
        r->capacity *= 4;
        r->heapAllocatedStuff = realloc(r->heapAllocatedStuff, r->capacity * sizeof(void*));
        assert(r->heapAllocatedStuff);
    }
    void *a = malloc(size);
    r->heapAllocatedStuff[r->size++] = a;
    return (char *) a;
}

void DeleteRegion(Region *r){
    for(int i = 0; i < r->size; i++) {
        free(r->heapAllocatedStuff[i]);
    }
    free(r->heapAllocatedStuff);
    free(r);
}