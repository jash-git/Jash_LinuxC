#include "base.hpp"
#include <cmath>
#include <iostream>

class b: public baseClass {
public:

    void test()
    {
        std::cout << "in b" << std::endl;
    }
};


// the class factories

extern "C" baseClass* create() {
    return new b;
}

extern "C" void destroy(baseClass* p) {
    delete p;
}
