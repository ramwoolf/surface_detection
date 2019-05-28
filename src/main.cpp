#ifndef MAJOR_VERSION_NUMBER
#define MAJOR_VERSION_NUMBER 0
#endif

#ifndef MINOR_VERSION_NUMBER
#define MINOR_VERSION_NUMBER 0
#endif


#include <iostream>
#include <fstream>
#include <vector>

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

struct Data
{
    float epsilon;
    unsigned long count_points;
    std::vector<Point_t> points_vector;
};

template <typename T>
class DataProvider
{
public:
    static DataProvider<T> &get_instance(T &in)
    {
        static DataProvider<T> instance{in};
        return instance;
    }

    Data get_data()
    {
        Data data;
        input >> data.epsilon;
        input >> data.count_points;
        data.points_vector.reserve(data.count_points);
        float x{0.0f};
        float y{0.0f};
        float z{0.0f};
        for (unsigned long i{0ul}; i < data.count_points; ++i)
        {
            input >> x >> y >> z;
            data.points_vector.emplace_back(x, y, z);
        }
        return data;
    }

private:
    T &input;

    DataProvider(T &in) : input(in) {}
    DataProvider(DataProvider<T> const &arg);
    DataProvider<T> &operator=(DataProvider<T> const &arg);

};

}

int main(int argc, char const *argv[])
{
    using namespace SurfaceDetection;
    std::fstream fs{"test_file.txt", std::ios::in};
    DataProvider<std::fstream> &file_input_data_provider = DataProvider<std::fstream>::get_instance(fs);
    // DataProvider<std::istream> &std_input_data_provider = DataProvider<std::istream>::get_instance(std::cin);

    return 0;
}
