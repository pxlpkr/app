#pragma once

typedef struct {
    int x;
    int y;
} CH_Point;

extern double CH_Point_GetMagnitude(CH_Point* p);
extern CH_Point CH_Point_Sum(CH_Point* a, CH_Point* b);