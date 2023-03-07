#include "vector.h"

CH_Vec2D CH_Vec2D_sum(CH_Vec2D* a, CH_Vec2D* b) {
    CH_Vec2D vector = {a->x + b->x, a->y + b->y};
    return vector;
}