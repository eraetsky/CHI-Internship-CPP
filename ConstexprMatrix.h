#pragma once
#include <string>
#include <array>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <ostream>

//decided to do a separate solution for matrix rank in a compile time since I met a lot of issues 
//while working with Matrix.h

template <typename T, std::size_t N> struct vector 
{
    using value_type = T;
    using size_type = std::size_t;
    static constexpr size_type size = N; 

    constexpr T& operator[](size_type i) noexcept { return array[i]; }

    constexpr const T& operator[](size_type i) const noexcept { return array[i]; }

    constexpr T* begin() noexcept { return array; }
    constexpr T* end() noexcept { return array + N; }
    constexpr const T* cbegin() const noexcept { return array; }
    constexpr const T* cend() const noexcept { return array + N; }

    T array[N]; 
};

template <typename... Args> constexpr decltype(auto) make_vector(Args... args) {
    return vector{ args... };
}

//uniform initialization
template <typename T, typename... U>
vector(T, U...)
-> vector<std::enable_if_t<(std::is_same_v<T, U> && ...), T>,
    1 + sizeof...(U)>;

template <typename T, std::size_t N, std::size_t M> struct matrix 
{
    using value_type = T;
    using size_type = std::size_t;
    static constexpr size_type ncols = N;
    static constexpr size_type nrows = M;   

    constexpr T* operator[](std::size_t i) { return arrays[i]; }
    constexpr T const* operator[](std::size_t i) const { return arrays[i]; }

    T arrays[N][M]; 
};


template <typename T, std::size_t M, std::size_t N>
constexpr std::tuple<matrix<T, M, N>, std::size_t, T>
gauss_jordan_impl(matrix<T, M, N> m, T tolerance) {

    // Define function for determining if an element is negligible
    auto negligible = [&tolerance](const T& v) { return abs(v) < tolerance; };

    T det = 1;
    std::size_t rank = 0;
    std::size_t i = 0, j = 0;
    while (i < M && j < N) {
        // Choose largest magnitude as pivot to avoid adding different magnitudes
        for (std::size_t ip = i + 1; ip < M; ++ip) {
            if (abs(m[ip][j]) > abs(m[i][j])) {
                for (std::size_t jp = 0; jp < N; ++jp) {
                    auto tmp = m[ip][jp];
                    m[ip][jp] = m[i][jp];
                    m[i][jp] = tmp;
                }
                det *= -1;
                break;
            }
        }

        // If m_ij is still 0, continue to the next column
        if (!negligible(m[i][j])) {
            // Scale m_ij to 1
            auto s = m[i][j];
            for (std::size_t jp = 0; jp < N; ++jp)
                m[i][jp] /= s;
            det /= s;

            // Eliminate other values in the column
            for (std::size_t ip = 0; ip < M; ++ip) {
                if (ip == i)
                    continue;
                if (!negligible(m[ip][j])) {
                    auto s = m[ip][j];
                    for (std::size_t jp = 0; jp < N; ++jp)
                        m[ip][jp] -= s * m[i][jp];
                }
            }

            ++rank;

            ++i;
        }
        ++j;
    }
    det = (rank == M) ? det : 0;
    return { m, rank, det };
}

//not implemented - do not understood how to compute mars
template <typename T, std::size_t M, std::size_t N>
constexpr std::tuple<matrix<T, M, N>, std::size_t, T>
gauss_jordan_impl(const matrix<T, M, N>& m) {
    T tol = std::max(N, M) * std::numeric_limits<T>::epsilon() * mars(m);   //mars stands for maximum absolute row sum norm of a matrix.
    return gauss_jordan_impl(m, tol);
}
//used this article: https://fncbook.github.io/v1.0/linsys/norms.html#:~:text=The%20infinity%20norm%20of%20a,all%20after%20taking%20absolute%20values.&text=In%20words%2C%20the%20infinity%20norm,is%20the%20maximum%20column%20sum.
//hadn't enough time to understand how it may work

template <typename T, std::size_t M, std::size_t N>
constexpr std::size_t rank(const matrix<T, M, N>& m) {
    return std::get<1>(gauss_jordan_impl(m));
}
