#include "base.hpp"
#include <cmath>
#include <iostream>

class c: public baseClass {
public:

    void test()
    {
        std::cout << "in c" << std::endl;
    }
};


// the class factories

extern "C" baseClass* create() {
    return new c;
}

extern "C" void destroy(baseClass* p) {
    delete p;
}
