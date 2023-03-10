#pragma once

typedef struct {
    int size;
    int len;
    void** at;
} CH_Array;

extern CH_Array* CH_Array_Create(void);
extern void CH_Array_Destroy(CH_Array* array_p);
extern void CH_Array_Append(CH_Array* array_p, void* value);
extern void CH_Array_Remove(CH_Array* array_p, int index);