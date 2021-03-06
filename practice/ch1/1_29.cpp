#include <iostream>
#include <fstream>
#include "algs_type.h"
#include "algstl_array.h"
#include "algsstring.h"
#include "binary_search.h"

using namespace std;
using namespace algs;
using namespace algstl;

int main(int argc, char *argv[])
{
    Array<Int> a = {1, 4, 4, 4, 4, 4, 4};

    cout << BinarySearch::countLess(4, a) << endl;
    cout << BinarySearch::countLarger(4, a) << endl;
    cout << BinarySearch::count(4, a) << endl;

    return 0;
}
