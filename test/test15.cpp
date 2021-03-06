#include <iostream>
#include "algsstring.h"
#include "algsnum.h"
#include "algsout.h"

using namespace algs;
using namespace std;

int main(int argc, char *argv[])
{
    String a("123");

    Int x = Num::parseInt(a);

    cout << x + 1 << endl;

    cout << Num::toString(x + 2) << endl;
    cout << Num::toString(21.1) << endl;
    cout << Num::toString(1.1111223122) << endl;

    AlgsStdOut::printf("%.4f\n", 12.33134);

    return 0;
}
