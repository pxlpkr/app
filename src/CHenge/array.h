#pragma once

typedef struct {
    int size;
    int len;
    void** at;
} CH_Array;

CH_Array* CH_Array_Create(void);
void CH_Array_Destroy(CH_Array* array_p);
void CH_Array_Append(CH_Array* array_p, void* value);
void CH_Array_Remove(CH_Array* array_p, int index);