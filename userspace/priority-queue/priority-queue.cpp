#include <iostream>
#include <ctime>
#include <limits>
#include <stdexcept>

struct heap {
public:
    heap(int cap)
        : capacity(cap), size(0) {
        arr = new int[capacity + 1];
        arr[0] = std::numeric_limits<int>::min();
    }
    bool full() const { return size == capacity; }
    bool empty() const { return size == 0; }
    void insert(int val);
    int delmin();

private:
    int capacity;
    int size;
    int *arr;
};

void heap::insert(int val)
{
    if (full()) {
        throw std::overflow_error("heap is full");
    }
    int i = 0;
    size++;
    for (i = size; arr[i/2] > val; i /= 2) {
        arr[i] = arr[i/2];
    }
    arr[i] = val;
}

int heap::delmin()
{
    if (empty()) {
        throw std::underflow_error("heap is empty");
    }
    int first = arr[1];
    int last = arr[size--];
    int child = 0;
    int i = 0;
    for (i = 1; i * 2 <= size; i = child) {
        child = i * 2;
        if (child + 1 < size && arr[child + 1] < arr[child]) {
            child++;
        }
        if (last > arr[child]) {
            arr[i] = arr[child];
        } else {
            break;
        }
    }
    arr[i] = last;
    return first;
}

int main(int argc, char *argv[])
{
    heap h(100);
    for (int i = 0; i < 100; ++i) {
        h.insert(rand() % 100);
    }

	for (int i = 0; i < 100; ++i) {
        std::cout << h.delmin() << " ";
    }
    std::cout <<std::endl;
}
