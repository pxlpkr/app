#include <stdlib.h>
#include <math.h>

#include "point.h"

double CH_Point_GetMagnitude(CH_Point* p) {
    return sqrt(pow(p->x, 2) + pow(p->y, 2));
}

CH_Point CH_Point_Sum(CH_Point* a, CH_Point* b) {
    return (CH_Point) {a->x + b->x, a->y + b->y};
}