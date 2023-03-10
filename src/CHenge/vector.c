#include <stdlib.h>
#include <math.h>

#include "vector.h"

CH_Vector* CH_Vector_Create(double x, double y, double z) {
    CH_Vector* vec = malloc(sizeof(CH_Vector));

    vec->x = x;
    vec->y = y;
    vec->z = z;

    return vec;
}

void CH_Vector_Destroy(CH_Vector* vector) {
    free(vector);
}

CH_Vector* CH_Vector_Sum(CH_Vector* a, CH_Vector* b) {
    return CH_Vector_Create(a->x + b->x, a->y + b->y, a->z + b->z);
}

double CH_Vector_GetMagnitude(CH_Vector* v) {
    return sqrt(pow(v->x, 2) + pow(v->y, 2) + pow(v->z, 2));
}

CH_Vector* CH_Vector_CrossProduct(CH_Vector* a, CH_Vector* b) {
    return CH_Vector_Create(
        a->y * b->z - a->z * b->y,
        a->z * b->x - a->x * b->z,
        a->x * b->y - a->y * b->x
    );
}