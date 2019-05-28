#ifndef MAJOR_VERSION_NUMBER
#define MAJOR_VERSION_NUMBER 0
#endif

#ifndef MINOR_VERSION_NUMBER
#define MINOR_VERSION_NUMBER 0
#endif


#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

namespace SurfaceDetection
{

enum class PointOwning{Road, Not_Road, Undefined};

struct Point_t
{
    float x;
    float y;
    float z;
    mutable int cluster_number;

    Point_t(float x_arg, float y_arg, float z_arg)
    : x{x_arg}, y{y_arg}, z{z_arg}, cluster_number{-1}
    {}

    friend std::istream &operator>>(std::istream &in, Point_t &arg);
};

std::istream &operator>>(std::istream &in, Point_t &arg)
{
    return in >> arg.x >> arg.y >> arg.z;
}

struct Data
{
    float surface_bias;
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
        input >> data.surface_bias;
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

std::vector<Point_t> range_query(Data const &data, Point_t const &lhs_point)
{
    double const epsilon{1.0};
    auto dist_func = [](Point_t const &lhs, Point_t const &rhs)
    {
        auto delta_x{std::abs(lhs.x - rhs.x)};
        auto delta_y{std::abs(lhs.y - rhs.y)};
        auto delta_z{std::abs(lhs.z - rhs.z)};
        return std::sqrt(delta_x*delta_x + delta_y*delta_y + delta_z*delta_z);
    };
    std::vector<Point_t> neighbors;
    for (auto &rhs_point : data.points_vector)
    {
        if (dist_func(lhs_point, rhs_point) < epsilon)
        {
            neighbors.push_back(rhs_point);
        }
    }
    return neighbors;
}

void dbscan(Data const &data)
{
    int const min_points_for_cluster{5};
    int cluster_number{0};
    for (auto& point : data.points_vector)
    {
        if (point.cluster_number >= 0) continue;
        auto neighbors = range_query(data, point);
        if (neighbors.size() < min_points_for_cluster)
        {
            point.cluster_number = 0;
            continue;
        }
        ++cluster_number;
        point.cluster_number = cluster_number;
        //      Seed set S=N \ {P}                                /* Соседи для расширения */
        //      for each point Q in S {                             /* Обрабатываем каждую зачаточную точку */
        //      if label(Q)=Noise then label(Q)=C            /* Заменяем метку Шум на Край */
        //      if label(Q) ≠ undefined then continue            /* Была просмотрена */
        //      label(Q)=C                                     /* Помечаем соседа */
        //      Neighbors N=RangeQuery(DB, distFunc, Q, eps)   /* Находим соседей */
        //      if|N|≥ minPts then {                           /* Проверяем плотность */
        //         S=S ∪ N                                     /* Добавляем соседей в набор зачаточных точек */
        //      }
        //   }
    }
}

}

int main(int argc, char const *argv[])
{
    using namespace SurfaceDetection;
    std::fstream fs{"../test/test_file.txt", std::ios::in};
    DataProvider<std::fstream> &file_input_data_provider = DataProvider<std::fstream>::get_instance(fs);
    auto data{file_input_data_provider.get_data()};
    dbscan(data);
    // DataProvider<std::istream> &std_input_data_provider = DataProvider<std::istream>::get_instance(std::cin);

    return 0;
}
