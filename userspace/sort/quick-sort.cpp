#include <iostream>
#include <algorithm>

const int CUTOFF = 3;

void insert_sort(int array[], int size)
{
    for (int i = 1; i < size; ++i) {
        int val = array[i];
        int j = i;
        for (; j > 0; --j) {
            if (array[j - 1] > val) {
                array[j] = array[j - 1];
            } else {
                break;
            }
        }
        array[j] = val;
    }
}

int median3(int array[], int left, int right)
{
    int mid = (left + right) / 2;
    using std::swap;
    if (array[left] > array[mid]) {
        swap(array[left], array[mid]);
    }
    if (array[left] > array[right]) {
        swap(array[left], array[right]);
    }
    if (array[mid] > array[right]) {
        swap(array[mid], array[right]);
    }
    swap(array[mid], array[right - 1]);
    return array[right - 1];
}

void quick_sort(int array[], int left, int right)
{
    if (left + CUTOFF <= right) {
        using std::swap;
        int median = median3(array, left, right);
        int i = left;
        int j = right - 1;
        while (1) {
            while (array[++i] < median);
            while (array[--j] > median);
            if (i < j) {
                swap(array[i], array[j]);
            } else {
                break;
            }
        }
        swap(array[i], array[right - 1]);
        quick_sort(array, left, i - 1);
        quick_sort(array, i + 1, right);
    } else {
        insert_sort(array + left, right - left + 1);
    }
}

int main(int argc, char *argv[])
{
    int array[] = {89, 67, 45, 78, 89, 56, 12, 1, 87, 10};
    quick_sort(array, 0, 9);
    for (int i = 0; i < 10; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
