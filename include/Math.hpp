#pragma once

#include <array>

namespace Math
{

struct Vertex
{
    float x, y, z;
};

enum class Axis
{
    x,
    y,
    z
};

template <int N, int M> class Matrix
{
  private:
    std::array<float, N * M> data = {};

  public:
    Matrix();
    Matrix<N, M> operator+(const Matrix<N, M>& other) const;
    float& operator()(int row, int col);
    const float& operator()(int row, int col) const;
    template <int N_, int M_, int K>
    friend Matrix<N_, K> operator*(
        const Matrix<N_, M_>& left, const Matrix<M_, K>& right
    );
};

class TransformController
{
};

} // namespace Math