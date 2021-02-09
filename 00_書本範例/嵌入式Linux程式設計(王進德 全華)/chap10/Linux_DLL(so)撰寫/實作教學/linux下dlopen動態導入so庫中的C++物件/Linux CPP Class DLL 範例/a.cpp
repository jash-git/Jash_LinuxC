#include "base.hpp"
#include <cmath>
#include <iostream>

class a: public baseClass {
public:

    void test()
    {
        std::cout << "in a" << std::endl;
    }
};


// the class factories

extern "C" baseClass* create() {
    return new a;
}

extern "C" void destroy(baseClass* p) {
    delete p;
}
