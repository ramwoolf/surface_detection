#ifndef MAJOR_VERSION_NUMBER
#define MAJOR_VERSION_NUMBER 0
#endif

#ifndef MINOR_VERSION_NUMBER
#define MINOR_VERSION_NUMBER 0
#endif


#include <iostream>

namespace SurfaceDetection
{

enum class PointOwning{Road, Not_Road, Undefined};

struct Point_t
{
    float x;
    float y;
    float z;
    PointOwning status;

    Point_t(float x_arg, float y_arg, float z_arg)
    : x{x_arg}, y{y_arg}, z{z_arg}, status{PointOwning::Undefined}
    {}

    friend std::istream &operator>>(std::istream &in, Point_t &arg);
};

std::istream &operator>>(std::istream &in, Point_t &arg)
{
    return in >> arg.x >> arg.y >> arg.z;
}

};

int main(int argc, char const *argv[])
{
    using namespace SurfaceDetection;


    return 0;
}
