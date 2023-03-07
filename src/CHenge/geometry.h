#include "vector.h"

typedef struct {
    CH_Vec2D vertices[3];
} CH_Geometry;

CH_Geometry* CH_Geometry_Create(void);
void CH_Geometry_Destroy(CH_Geometry* array_p);