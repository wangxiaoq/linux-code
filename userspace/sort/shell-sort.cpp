#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

void shell_sort(std::vector<int> &iv)
{
    for (int increment = iv.size() / 2; increment > 0; increment /= 2) {
        for (int i = increment; i < iv.size(); ++i) {
            int j = i;
            int tmp = iv[j];
            for (; j >= increment; j -= increment) {
                if (iv[j - increment] > tmp) {
                    iv[j] = iv[j - increment];
                } else {
                    break;
                }
            }
            iv[j] = tmp;
        }
    }
}

int main(int argc, char *argv[])
{
    std::vector<int> ivec;
    for (int i = 0; i < 100; ++i) {
        ivec.push_back(random() % 100);
    }

    shell_sort(ivec);
    std::for_each(ivec.cbegin(), ivec.cend(), [](int v)->void { std::cout << v << " "; });
    std::cout << std::endl;
}
