#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void selection_sort(int arr[], int n, int *steps, int *comp, int *swaps);
void improved_selection(int arr[], int n, int *steps, int *comp, int *swaps);
void bubble_sort(int arr[], int n, int *steps, int *comp, int *swaps);

int main() {
    int n = 0;
    while(n < 1) {
        printf("Nhap kich co mang: ");
        scanf("%d", &n);
    }

    int arr[n], input_mode = -1, sort_mode = -1;
    while(input_mode != 0 && input_mode != 1) {
        printf("Nhap che do nhap (0 = thu cong, 1 = ngau nhien): ");
        scanf("%d", &input_mode);
    }

    if(input_mode) {
        srand(time(NULL));
        printf("Mang truoc khi sap xep:\n");

        for(int i = 0; i < n; i++) {
            arr[i] = rand() % 1000;
            printf("%d ", arr[i]);
        }

        printf("\n\n");
    } else {
        for(int i = 0; i < n; i++) {
            printf("Nhap phan tu thu %d: ", i);
            scanf("%d", &arr[i]);
        }
    }

    while(sort_mode < 0 || sort_mode > 2) {
        printf("Chon thuat toan sap xep (0 = selection sort, 1 = improved selection sort, 2 = bubble sort): ");
        scanf("%d", &sort_mode);
    }

    int steps = 0, comparisons = 0, swaps = 0;
    if(sort_mode == 0) selection_sort(arr, n, &steps, &comparisons, &swaps);
    else if(sort_mode == 1) improved_selection(arr, n, &steps, &comparisons, &swaps);
    else if(sort_mode == 2) bubble_sort(arr, n, &steps, &comparisons, &swaps);

    printf("Steps: %d\nComparisons: %d\nSwaps: %d\nMang sau sap xep:\n", steps, comparisons, swaps);
    for(int i = 0; i < n; i++) printf("%d ", arr[i]);


    return 0;
}

void improved_selection(int arr[], int n, int *steps, int *comp, int *swaps) {
    int min, max, temp;

    for(int i = 0; i <= (n - 1) / 2; i++) {
        min = i;
        max = n - 1 - i;
        int isSorted = 1;

        for(int j = i; j < n - i; j++, (*steps)++, (*comp) += 2) {
            if(arr[j] < arr[min]) {
                min = j;
                isSorted = 0;
            }
            if(arr[j] > arr[max]) {
                max = j;
                isSorted = 0;
            }
        }

        if(isSorted) break;

        if(min != i) {
            temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;

            if(max == i) max = min;

            (*swaps)++;
            (*comp)++;
        }

        if(max != (n - 1 - i)) {
            temp = arr[n - 1 - i];
            arr[n - 1 - i] = arr[max];
            arr[max] = temp;

            (*swaps)++;
        }

        (*comp) += 3;
    }
}

void selection_sort(int arr[], int n, int *steps, int *comp, int *swaps) {
        int min, max, temp;

    for(int i = 0; i < n - 1; i++) {
        min = i;
        int isSorted = 1;

        for(int j = i; j < n; j++, (*steps)++, (*comp) ++) {
            if(arr[j] < arr[min]) {
                min = j;
                isSorted = 0;
            }
        }

        if(isSorted) break;

        if(min != i) {
            temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;

            (*swaps)++;
        }

        (*comp) += 2;
    }
}

void bubble_sort(int arr[], int n, int *steps, int *comp, int *swaps) {
    int temp;

    for(int i = 0; i < n - 1; i++) {
        int isSorted = 1;

        for(int j = 0; j < n - 1 - i; j++, (*steps)++, (*comp) ++) {
            if(arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                (*swaps)++;

                isSorted = 0;
            }
        }

        (*comp)++;
        if(isSorted) break;
    }
}