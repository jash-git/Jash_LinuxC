#include "base.hpp"
#include <iostream>
#include <dlfcn.h>

using std::cout;
using std::cerr;

void *load_so(const char*so_path)
{
    void* dl = dlopen(so_path, RTLD_LAZY);
    if (!dl) {
        cerr << "Cannot load library: " << dlerror() << '\n';
    }
    return dl;
}

int test_so(void *dl)
{
    if (!dl) {
        return 0;
    }

    create_t create_func = (create_t) dlsym(dl, "create");
    destroy_t destroy_func = (destroy_t) dlsym(dl, "destroy");
    if (!create_func || !destroy_func) {
        cerr << "Cannot load symbols: " << dlerror() << '\n';
        return -1;
    }
    baseClass* base = create_func();
    base->test();
    destroy_func(base);

    dlclose(dl);
	return 0;
}

int main()
{
    void *dl  = load_so("./a.so");  
    test_so(dl);

    dl  = load_so("./b.so");  
    test_so(dl);

    dl  = load_so("./c.so");  
    test_so(dl);
    return 0;
}
