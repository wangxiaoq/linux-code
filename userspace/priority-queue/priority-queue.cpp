#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include <iterator>

class binheap {
public:
    using size_type = std::vector<int>::size_type;
    binheap() { vec.push_back(std::numeric_limits<int>::min()); }
    binheap(const std::initializer_list<int> &il);
    bool empty() const { return size() == 0; }
    void insert(int val);
    int delmin();
private:
    std::vector<int> vec;
    size_type size() const { return vec.size() - 1; }
};

binheap::binheap(const std::initializer_list<int> &il)
    : vec(1, std::numeric_limits<int>::min())
{
    copy(il.begin(), il.end(), back_inserter(vec));
    size_type s = size();
    for (int i = s / 2; i > 0; --i) {
        int val = vec[i];
        int j = 0;
        int child = 0;
        for (j = i; j * 2 <= s; j = child) {
            child = j * 2;
            if (child + 1 <= s && vec[child + 1] < vec[child]) {
                child++;
            }

            if (vec[child] < val) {
                vec[j] = vec[child];
            } else {
                break;
            }
        }
        vec[j] = val;
    }
}

void binheap::insert(int val)
{
    vec.push_back(val);
    size_type s = size();
    int i = s;
    for (; vec[i / 2] > val; i /= 2) {
        vec[i] = vec[i / 2];
    }
    vec[i] = val;
}

int binheap::delmin()
{
    size_type s = size();
    int first = vec[1];
    int last = vec[s];
    vec.pop_back();
    s = size();
    int i = 0;
    int child = 0;
    for (i = 1; i * 2 <= s; i = child) {
        child = i * 2;
        if (child + 1 <= s && vec[child + 1] < vec[child]) {
            child++;
        }

        if (vec[child] < last) {
            vec[i] = vec[child];
        } else {
            break;
        }
    }
    vec[i] = last;
    return first;
}

int main(int argc, char *argv[])
{
    binheap heap;
    for (int i = 0; i < 100; ++i) {
        heap.insert(random() % 100);
    }

    for (int i = 0; i < 100; ++i) {
        std::cout << heap.delmin() << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    binheap heap1({3,5,7,4,67,32,543,1,43,5567,98,22});
    while (!heap1.empty()) {
        std::cout << heap1.delmin() << " ";
    }
    std::cout << std::endl;

    return 0;
}
