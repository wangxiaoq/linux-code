#include <iostream>

class Singleton {
friend std::ostream &operator<<(std::ostream &out, const Singleton &rhs);
public:
    static Singleton instance;
    static Singleton *getInstance() { return &instance; }
    void inc() { value++; }
private:
    int value;
    Singleton()
        : value(0) {};
};

Singleton Singleton::instance;

std::ostream &operator<<(std::ostream &out, const Singleton &rhs)
{
    out << rhs.value;
    return out;
}

int main(int argc, char *argv[])
{
    Singleton *s1 = Singleton::getInstance();
    s1->inc();
    Singleton *s2 = Singleton::getInstance();
    std::cout << *s2 << std::endl;

    return 0;
}
