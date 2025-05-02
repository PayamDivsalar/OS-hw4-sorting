#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct sarray {
    int *inp;
    int n;
} sarray;

typedef struct twoarray {
    int *inpl;
    int *inpr;
    int *merged;
    int s;
} twoarray;

void* littlesort(void* arg) {
    sarray *args = (sarray*)arg;
    for (int i = 1; i < args->n; ++i) {
        int key = args->inp[i];
        int j = i - 1;
        while (j >= 0 && args->inp[j] > key) {
            args->inp[j + 1] = args->inp[j];
            j--;
        }
        args->inp[j + 1] = key;
    }
    return NULL;
}

void* mergew(void* arg) {
    twoarray *args = (twoarray*)arg;
    int left_size = args->s / 2;
    int right_size = args->s - left_size;

    args->merged = malloc(sizeof(int) * args->s);
    if (args->merged == NULL) {
        return NULL;
    }

    int i = 0, j = 0, k = 0;
    while (i < left_size && j < right_size) {
        if (args->inpl[i] <= args->inpr[j]) {
            args->merged[k++] = args->inpl[i++];
        } else {
            args->merged[k++] = args->inpr[j++];
        }
    }

    while (i < left_size) {
        args->merged[k++] = args->inpl[i++];
    }

    while (j < right_size) {
        args->merged[k++] = args->inpr[j++];
    }

    return NULL;
}

int main() {
    int n;
    scanf("%d", &n);
    int inp[n];
    for (int i = 0; i < n; i++) scanf("%d", &inp[i]);

    int mid = n / 2;
    int inp_l[mid];
    int inp_r[n - mid];

    for (int i = 0; i < mid; i++) inp_l[i] = inp[i];
    for (int i = mid; i < n; i++) inp_r[i - mid] = inp[i];

    sarray *left_arg = malloc(sizeof(sarray));
    sarray *right_arg = malloc(sizeof(sarray));
    left_arg->inp = inp_l;
    left_arg->n = mid;
    right_arg->inp = inp_r;
    right_arg->n = n - mid;

    pthread_t left_sort_thread, right_sort_thread, merge_thread;
    pthread_create(&left_sort_thread, NULL, littlesort, left_arg);
    pthread_create(&right_sort_thread, NULL, littlesort, right_arg);

    pthread_join(left_sort_thread, NULL);
    pthread_join(right_sort_thread, NULL);

    twoarray *tt = malloc(sizeof(twoarray));
    tt->inpl = left_arg->inp;
    tt->inpr = right_arg->inp;
    tt->s = n;

    pthread_create(&merge_thread, NULL, mergew, tt);
    pthread_join(merge_thread, NULL);

    for (int i = 0; i < tt->s; i++) {
        printf("%d ", tt->merged[i]);
    }
    printf("\n");


    return 0;
}