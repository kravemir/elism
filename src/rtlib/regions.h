//
// Created by miroslav on 4/15/17.
//

#ifndef BP_REGIONS_H
#define BP_REGIONS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Region Region;

Region *NewRegion();
char *RegionAlloc(Region *r, size_t size);
void DeleteRegion(Region *r);

#ifdef __cplusplus
}
#endif

#endif //BP_REGIONS_H
