#include <stdlib.h>
#include <stdbool.h>

int cmpfnc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

bool is_sequence(int arr[], int size) {
    /* checks if passed 'arr' is sequence
     * starting from 1 to 'size'. Ex. if
     * size is 5 'arr' must contain
     * elements { 1, 2, 3, 4, 5} (sort
     * doesn't matter).
     */
    bool is_seq = true;
    int i;

    qsort(arr, size, sizeof(int), cmpfnc);
    for (i = 0; i < size; ++i) {
        if (arr[i] != i+1)
            return false;
    }

    return is_seq;
}
