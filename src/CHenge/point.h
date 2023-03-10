#pragma once

typedef struct {
    int x;
    int y;
} CH_Point;

extern CH_Point* CH_Point_Create(int x, int y);
extern void CH_Point_Destroy(CH_Point* pt);