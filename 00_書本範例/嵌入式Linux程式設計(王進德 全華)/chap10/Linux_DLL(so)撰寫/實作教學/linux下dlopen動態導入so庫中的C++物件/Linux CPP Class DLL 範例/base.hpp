#ifndef __base__
#define __base__

#include <iostream>

class baseClass {

public:
	virtual void test(){};
	virtual ~baseClass(){};
};

//typedef baseClass* create_t();
typedef baseClass* (*create_t)();
typedef void (*destroy_t)(baseClass*);

#endif
