#include <stdlib.h>

#include "point.h"

CH_Point* CH_Point_Create(int x, int y) {
    CH_Point* pt = malloc(sizeof(CH_Point));

    pt->x = x;
    pt->y = y;

    return pt;
}

void CH_Point_Destroy(CH_Point* pt) {
    free(pt);
}