#include <iostream>
#include "stdstring.h"

using namespace algs;
using namespace std;

int main(int argc, char *argv[])
{
#if 0
    String a;
    cout << a.c_str() << endl;

    String b("Hello world");
    cout << b.c_str() << endl;

    String c(a);
    cout << c.c_str() << endl;

    String d(b);
    cout << d.c_str() << endl;
#endif

    String e = "Hello world";

    cout << ("Hello " + e).c_str() << endl;
    cout << (e + "Hello ").c_str() << endl;
    cout << (e + "Hello " + e).c_str() << endl;
    cout << ("Hello " + e + " Hello").c_str() << endl;

    return 0;
}
