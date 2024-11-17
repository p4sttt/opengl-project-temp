#include "Math.hpp"
#include "Logger.hpp"
#include <cmath>

template <int N, int M> Math::Matrix<N, M>::Matrix()
{
    _data.fill(0.0f);
}

template <int N, int M>
Math::Matrix<N, M>::Matrix(std::initializer_list<std::initializer_list<float>> values)
{
    int row = 0;
    for (const auto& row_values : values)
    {
        int col = 0;
        for (const auto& value : row_values)
        {
            if (row < N && col < M)
            {
                (*this)(row, col) = value;
            }
            ++col;
        }
        ++row;
    }
}

template <int N, int M> float& Math::Matrix<N, M>::operator()(int row, int col)
{
    return _data[row * M + col];
}

template <int N, int M>
const float& Math::Matrix<N, M>::operator()(int row, int col) const
{
    return _data[row * M + col];
}

template <int N, int M>
Math::Matrix<N, M> Math::Matrix<N, M>::operator+(const Matrix<N, M>& other) const
{
    Math::Matrix<N, M> result;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            result(i, j) = (*this)(i, j) + other(i, j);
        }
    }

    return result;
}

template <int N_, int M_, int K>
Math::Matrix<N_, K> Math::operator*(
    const Math::Matrix<N_, M_>& left, const Math::Matrix<M_, K>& right
)
{
    Math::Matrix<N_, K> result;
    for (int i = 0; i < N_; ++i)
    {
        for (int j = 0; j < M_; ++j)
        {
            float current = 0;
            for (int k = 0; k < M_; ++k)
            {
                current += left(i, k) * right(k, j);
            }
            result(i, j) = current;
        }
    }

    return result;
}

template <int N, int M> const float* Math::Matrix<N, M>::Data() const
{
    return _data.data();
}

template <int N, int M> void Math::Matrix<N, M>::SetIdenity()
{
    for (int i = 0; i < N; ++i)
    {
        (*this)(i, i) = 1.0f;
    }
}

template class Math::Matrix<4, 4>;

Math::Transform::Transform()
{
    LOG << "Create Trasform" << '\n';
}

Math::Transform::~Transform()
{
    LOG << "Kill Trasform" << '\n';
}

void Math::Transform::Reset()
{
    position[Math::Axis::x] = 0.0f;
    position[Math::Axis::y] = 0.0f;
    position[Math::Axis::z] = 0.0f;

    rotation[Math::Axis::x] = 0.0f;
    rotation[Math::Axis::y] = 0.0f;
    rotation[Math::Axis::z] = 0.0f;

    scale[Math::Axis::x] = 1.0f;
    scale[Math::Axis::y] = 1.0f;
    scale[Math::Axis::z] = 1.0f;
}

void Math::Transform::Move(Math::Axis axis, bool positive)
{
    float step = positive ? _move_step : -_move_step;
    position[axis] += step;
}

void Math::Transform::Rotate(Math::Axis axis, bool positive)
{
    float step = positive ? _rotation_step : -_rotation_step;
    rotation[axis] += step;
}

void Math::Transform::Scale(Math::Axis axis, bool positive)
{
    float step = positive ? _scale_step : -_scale_step;

    scale[axis] += step;

    if (scale[axis] < 0.01f)
    {
        scale[axis] = 0.01f;
    }
}

Math::Matrix<4, 4> Math::Transform::GetMatrix() const
{
    Math::Matrix<4, 4> transformation_matrix;
    transformation_matrix.SetIdenity();

    Math::Matrix<4, 4> scale_matrix;
    for (int i = 0; i < 4; ++i)
        scale_matrix(i, i) = (i < 3) ? scale[i] : 1.0f;

    float sin_x = sinf(rotation[Math::Axis::x]);
    float cos_x = cosf(rotation[Math::Axis::x]);
    Math::Matrix<4, 4> rotation_x = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, cos_x, -sin_x, 0.0f},
        {0.0f, sin_x, cos_x, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };
    float sin_y = sinf(rotation[Math::Axis::y]);
    float cos_y = cosf(rotation[Math::Axis::y]);
    Math::Matrix<4, 4> rotation_y = {
        {cos_y, 0.0f, sin_y, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {-sin_y, 0.0f, cos_y, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };

    float sin_z = sinf(rotation[Math::Axis::z]);
    float cos_z = cosf(rotation[Math::Axis::z]);
    Math::Matrix<4, 4> rotation_z = {
        {cos_z, -sin_z, 0.0f, 0.0f},
        {sin_z, cos_z, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };

    transformation_matrix = rotation_z * rotation_y * rotation_x * scale_matrix;
    transformation_matrix(0, 3) = position[Math::Axis::x];
    transformation_matrix(1, 3) = position[Math::Axis::y];
    transformation_matrix(2, 3) = position[Math::Axis::z];

    return transformation_matrix;
}