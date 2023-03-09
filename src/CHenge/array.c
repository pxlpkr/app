#include <stdlib.h>

#include "array.h"

CH_Array* CH_Array_Create(void) {
    CH_Array* array_p = malloc(sizeof(CH_Array));
    array_p->len = 0;
    array_p->size = 1;

    void** content_p = (void**) malloc(array_p->size * sizeof(void*));
    array_p->at = content_p;

    return array_p;
}

void CH_Array_Destroy(CH_Array* array_p) {
    free(array_p);
}

/**
 * Doubles the size of the array in memory
 *
 * First, doubles the declared memory size of the struct and allocates
 * new memory equal to the new size. Then updates the old pointer to
 * the array with the newly expanded array.
 *
 * \param array_p pointer to the array
 */
void array_memory_extend(CH_Array* array_p) {
    array_p->size *= 2;
    void** content_p = (void**) realloc(array_p->at, array_p->size * sizeof(void*));
    array_p->at = content_p;
}

void CH_Array_Append(CH_Array* array_p, void* value) {
    if (array_p->len == array_p->size) array_memory_extend(array_p);

    array_p->at[array_p->len] = value;
    array_p->len++;
}

void CH_Array_Remove(CH_Array* array_p, int index) {
    if (index >= array_p->len) return;

    array_p->at[array_p->len] = 0;
    for (int i = index + 1; i < array_p->len; i++) {
        array_p->at[i - 1] = array_p->at[i];
        array_p->at[i] = 0;
    }

    array_p->len--;
}