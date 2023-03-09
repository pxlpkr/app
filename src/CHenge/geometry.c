#include <stdlib.h>

#include "geometry.h"

CH_Geometry* CH_Geometry_Create(CH_Vector* a, CH_Vector* b, CH_Vector* c) {
    CH_Geometry* geom = malloc(sizeof(CH_Geometry));

    geom->vertices[0] = a;
    geom->vertices[1] = b;
    geom->vertices[2] = c;

    return geom;
};

void CH_Geometry_Destroy(CH_Geometry* geometry) {
    for (int i = 0; i < 3; i++) {
        CH_Vector_Destroy(geometry->vertices[i]);
    }
    free(geometry);
}