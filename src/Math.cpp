#include "Math.hpp"

template <int N, int M> Math::Matrix<N, M>::Matrix()
{
    data.fill(0.0f);
}

template <int N, int M, int K>
Math::Matrix<N, K> operator*(
    const Math::Matrix<N, M>& left, const Math::Matrix<M, K>& right
)
{
    // TODO
}