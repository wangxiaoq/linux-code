#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

void insert_sort(std::vector<int> &iv)
{
    for (int i = 1; i < iv.size(); ++i) {
        int tmp = iv[i];
        int j = i;
        for (; j > 0 && iv[j - 1] > tmp; --j) {
            iv[j] = iv[j - 1];
        }
        iv[j] = tmp;
    }
}

int main(int argc, char *argv[])
{
    std::vector<int> ivec;
    for (int i = 0; i < 100; ++i) {
        ivec.push_back(random() % 100);
    }

    insert_sort(ivec);
    std::for_each(ivec.cbegin(), ivec.cend(), [](int v)->void { std::cout << v << " "; });
    std::cout << std::endl;
    return 0;
}
