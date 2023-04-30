#pragma once
#include <string>
#include <array>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <ostream>
#include "Row.h"

template <typename T, size_t N, size_t M> //tried to calculate rank here
class ConstMatrix
{
	using matrix_t = std::array<std::array<T, M>, N>;
	using row_t = std::array<T, M>;
private:
	 matrix_t m;
	 size_t rank = 0;
public:
	constexpr ConstMatrix() = default;
	constexpr row_t operator[] (size_t ind) const { return m[ind]; }
	constexpr ConstMatrix(std::initializer_list<T> init)
	{
		size_t ind = 0, jnd = 0;
		for (const auto& val : init)
		{
			if (jnd >= M) { jnd = 0; ind++; }
			if (ind >= N) { ind = 0; jnd++; }
			if (ind >= N || jnd >= M) break;
			m[ind][jnd] = val;
			++jnd;
		}
		/* Not complining :(
		for (size_t k = 0; k < std::min(N, M); ++k)
		{
			size_t i_max = k;
			for (size_t i = k + 1; i < N; ++i)
			{
				if (std::abs(m[i][k]) > std::abs(m[i_max][k]))
					i_max = i;
			}

			if (m[i_max][k] == 0)
				continue;

			if (i_max != k)
			{
				std::swap_ranges(m[k].begin(), m[k].end(), m[i_max].begin());
				rank++;
			}

			for (size_t i = k + 1; i < N; ++i)
			{
				T f = m[i][k] / m[k][k];
				for (size_t j = k + 1; j < M; ++j)
				{
					m[i][j] -= f * m[k][j];
				}
				m[i][k] = 0;
			}
			rank++;
		}
		*/
	}

};


template<typename T, size_t N, size_t M>
std::ostream& operator << (std::ostream& os, const ConstMatrix<T, N, M>& matrix)
{
	char sep = ';';
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			os << matrix[i][j] << sep;
		}
		os << std::endl;
	}
	return os;
}