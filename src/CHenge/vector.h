#pragma once

typedef struct {
    double x;
    double y;
    double z;
} CH_Vector;

extern CH_Vector* CH_Vector_Create(double x, double y, double z);
extern void CH_Vector_Destroy(CH_Vector* vector);

extern CH_Vector* CH_Vector_Sum(CH_Vector* a, CH_Vector* b);
extern double CH_Vector_GetMagnitude(CH_Vector* v);