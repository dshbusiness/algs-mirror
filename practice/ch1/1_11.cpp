#include <iostream>
#include "algs_type.h"
#include "algstl_array.h"

using namespace std;
using namespace algstl;

int main(int argc, char *argv[])
{
    Array<Array<Bool>> a(
    {
        {true, false, true},
        {false, true, false},
        {true, false, true}
    });

    for (auto &i: a)
    {
        for (auto &j: i)
        {
            cout << (j ? "*" : " ") << "  ";
        }
        cout << endl;
    }

    return 0;
}
