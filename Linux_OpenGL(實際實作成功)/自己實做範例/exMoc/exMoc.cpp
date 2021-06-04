#include "myClass1.h"
#include "myClass2.h"
int main(int argc, char *argv[])
{
myclass1 obj1;
myclass2 obj2;
QObject::connect(&obj2,SIGNAL(passValue(int)),&obj1,SLOT(displayDecimal(int)));
QObject::connect(&obj2,SIGNAL(passValue(int)),&obj1,SLOT(displayHex(int)));
obj2.displayDigit(30);
return 1;
}
