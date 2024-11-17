#pragma once

#include <array>

namespace Math
{

struct Vertex
{
    float x, y, z;
};

enum Axis
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
    Matrix(std::initializer_list<std::initializer_list<float>> values);

    float& operator()(int row, int col);
    const float& operator()(int row, int col) const;

    Matrix<N, M> operator+(const Matrix<N, M>& other) const;
    template <int N_, int M_, int K>
    friend Matrix<N_, K> operator*(
        const Matrix<N_, M_>& left, const Matrix<M_, K>& right
    );

    const float* Data() const;

    void SetIdenity();
};

template <int N_, int M_, int K>
Matrix<N_, K> operator*(const Matrix<N_, M_>& left, const Matrix<M_, K>& right);

class Transform
{
  private:
    float position[3] = {0.0f, 0.0f, 0.0f};
    float rotation[3] = {0.0f, 0.0f, 0.0f};
    float scale[3] = {1.0f, 1.0f, 1.0f};

    float _move_step = 0.01f;
    float _rotation_step = 0.01f;
    float _scale_step = 0.01f;

  public:
    Transform();
    ~Transform();

    void Reset();
    void Move(Math::Axis axis, bool positive);
    void Rotate(Math::Axis axis, bool positive);
    void Scale(Math::Axis axis, bool positive);

    Math::Matrix<4, 4> GetMatrix() const;
};

} // namespace Math