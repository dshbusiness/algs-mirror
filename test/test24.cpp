#include <iostream>
#include "geometry.h"

using namespace std;
using namespace algs;

int main(int argc, char *argv[])
{
    Point2D p;

    cout << p.x_ << endl;
    p.setX(10);
    cout << p.x_ << endl;

    return 0;
}
