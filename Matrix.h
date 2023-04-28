#pragma once
#include <string>
#include <optional>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <ostream>
#include "Row.h"

template<typename T>
struct dimension { static constexpr size_t value = 0; };

template<typename T, typename... V>
struct dimension<std::vector<T, V...>> {
	static constexpr std::size_t value = 1 + dimension<T>::value;
};

template<typename T, typename... V>
struct dimension<std::vector<Row<T>, V...>> {
	static constexpr std::size_t value = 2 + dimension<T>::value;
};

using uint = unsigned int;

template <typename T = int>
class Matrix
{
	using row_t = Row<T>;
	using matrix_t = std::vector<row_t>;
	using comparator_t = bool (*)(const T&, const T&);
	
private:
	size_t nrows;
	size_t ncols;
	matrix_t m;
	using rank_t = dimension<decltype(m)>;
	size_t rank = rank_t::value; //first attempt - here rank is the level of nestin - for 2d matrix always equal 2
public:
	Matrix() = default;
	Matrix(size_t d1, size_t d2) : nrows(d1), ncols(d2), m(d1, row_t(d2)) {}
	Matrix(const Matrix& matrix) : nrows(matrix.nrows), ncols(matrix.ncols), m(matrix.m) {}
    Matrix(size_t d1, size_t d2, std::initializer_list<std::initializer_list<T>> il);
	row_t& operator[] (uint);
	const row_t& operator[] (uint) const; 
	void resize(size_t, size_t);
	void sortRow(uint, comparator_t = default_cmp); //default_cmp in Row.h
	void sortAllRows(comparator_t = default_cmp);
	void sortColumn(uint, comparator_t = default_cmp);
	void sortAllColumns(comparator_t = default_cmp);
	void fill(const T&);
	size_t getRows() const;
	size_t getCols() const;
	size_t getRank() const;
	Matrix plusOrMinus(const Matrix&, bool);
	Matrix operator+(const Matrix&);
	Matrix operator-(const Matrix&);
};

template<typename T>
 Matrix<T>::Matrix(size_t d1, size_t d2, std::initializer_list<std::initializer_list<T>> il): nrows(d1), ncols(d2), m(d1,Row<T>(d2))
{
	auto it1 = il.begin();
	for (size_t i = 0; i < d1 && it1 != il.end(); ++i, ++it1) {
		auto it2 = it1->begin();
		for (size_t j = 0; j < d2 && it2 != it1->end(); ++j, ++it2) {
			this->m[i][j] = *it2;
		}
	}
}

template <typename T>
Row<T>& Matrix<T>::operator[] (uint ind)
{
	try {
		if (ind >= nrows)
		{
			std::string exc_message = "Exception: Index out of range. Found index " + std::to_string(ind) +
				", while dimension is " + std::to_string(nrows);
			throw std::range_error(exc_message);
		}
		return this->m[ind];
		
	}
	catch (std::range_error err) { std::cerr << err.what() << std::endl; static Row<T> invalid_output; return invalid_output; }
}

template <typename T>
const Row<T>& Matrix<T>::operator[] (uint ind) const
{
	try {
		if (ind >= nrows)
		{
			std::string exc_message = "Exception: Index out of range. Found index " + std::to_string(ind) +
				", while dimension is " + std::to_string(nrows);
			throw std::range_error(exc_message);
		}
		return this->m[ind];
		
	}
	catch (std::range_error err) { std::cerr << err.what() << std::endl; static Row<T> invalid_output; return invalid_output; }
}

template <typename T>
void Matrix<T>::resize(size_t d1, size_t d2) //resize only upwards
{
	try {
		if (d1 <= this->nrows || d2 <= this->ncols) throw std::range_error("Exception: Possible lose of data while resizing matrix");
		else 
		{
			for (auto it = this->m.begin(); it != this->m.end(); ++it)
				it->resize(d2);
			this->m.resize(d1, Row<T>(d2));
			this->nrows = d1;
			this->ncols = d2;
		}
	}
	catch (std::range_error err) { std::cerr << err.what() << std::endl; return; }
}


template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other)
{
	return plusOrMinus(other, true);
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other)
{
	return plusOrMinus(other, false);
}

template <typename T>
Matrix<T> Matrix<T>::plusOrMinus(const Matrix<T>& other, bool is_plus)
{
	T cur1, cur2;
	Matrix<T> res(std::max(this->nrows, other.nrows), std::max(this->ncols, other.ncols));
	for (size_t i = 0; i < res.nrows; i++)
	{
		for (size_t j = 0; j < res.ncols; j++)
		{
			if (i < this->nrows && j < this->ncols) cur1 = this->m[i][j];
			else cur1 = 0;
			if (i < other.nrows && j < other.ncols) cur2 = other.m[i][j];
			else cur2 = 0;
			res[i][j] = is_plus ? (cur1 + cur2) : (cur1 - cur2);
		}
	}
	return res;
}

Matrix<std::string> Matrix<std::string>::operator+(const Matrix<std::string>& other) //plus operator for Matrix<std::string> to concat
{
	std::string cur1, cur2;
	Matrix<std::string> res(std::max(this->nrows, other.nrows), std::max(this->ncols, other.ncols));
	for (size_t i = 0; i < res.nrows; i++)
	{
		for (size_t j = 0; j < res.ncols; j++)
		{
			if (i < this->nrows && j < this->ncols) cur1 = this->m[i][j];
			else cur1 = "";
			if (i < other.nrows && j < other.ncols) cur2 = other.m[i][j];
			else cur2 = "";
			res[i][j] = cur1 + cur2;
		}
	}
	return res;
}

template<typename T>
void Matrix<T>::sortRow(uint ind, bool (*cmp)(const T& a, const T& b))
{
	this->m[ind].sortRow(cmp);
}

template<typename T>
void Matrix<T>::sortAllRows(bool (*cmp)(const T& a, const T& b))
{
	for (auto it = this->m.begin(); it != this->m.end(); ++it)
		it->sortRow(cmp);
}

template <typename T>
void Matrix<T>::fill(const T& value)
{
	for (auto it = this->m.begin(); it != this->m.end(); ++it)
		it->fill(value);
}

template <typename T>
void Matrix<T>::sortColumn(uint ind, bool (*cmp)(const T& a, const T& b))
{
	std::vector<T> column;
	for (size_t i = 0; i < this->nrows; ++i) 
		column.push_back(this->m[i][ind]);
	
	std::sort(column.begin(), column.end(), cmp);

	for (size_t i = 0; i < this->nrows; ++i) 
		this->m[i][ind] = column[i];
	
}

template<typename T>
void Matrix<T>::sortAllColumns(bool (*cmp)(const T& a, const T& b))
{
	for (size_t i = 0; i < ncols; ++i)
		this->sortColumn(i, cmp);
}

template<typename T>
size_t Matrix<T>::getRank() const
{
	return this->rank;  
}

template<typename T>
size_t Matrix<T>::getRows() const
{
	return this->nrows;
}

template<typename T>
size_t Matrix<T>::getCols() const
{
	return this->ncols;
}

//sfinae
template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0> //output operator for integral types
std::ostream& operator<< (std::ostream& os, const Matrix<T>& matrix)
{
	size_t nr = matrix.getRows();
	size_t nc = matrix.getCols();
	char sep = ';';
	for (size_t i = 0; i < nr; ++i) {
		for (size_t j = 0; j < nc; ++j) {
			os << matrix[i][j] << sep;
		}
		os << std::endl;
	}
	return os;
}

//sfinae
template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, int>::type = 0> //output operator for std::string
std::ostream& operator<< (std::ostream& os, const Matrix<T>& matrix)
{
	size_t nr = matrix.getRows();
	size_t nc = matrix.getCols();
	char sep = '|';
	for (size_t i = 0; i < nr; ++i) {
		for (size_t j = 0; j < nc; ++j) {
			os << matrix[i][j].length() << sep;
		}
		os << std::endl;
	}
	return os;
}

//sfinae
template<typename T, typename std::enable_if<!std::is_same<T, std::string>::value && !std::is_integral<T>::value, int>::type = 0> //for the rest
std::ostream& operator<< (std::ostream& os, const Matrix<T>& matrix)
{
	size_t nr = matrix.getRows();
	size_t nc = matrix.getCols();
	char sep = '|';
	for (size_t i = 0; i < nr; ++i) {
		for (size_t j = 0; j < nc; ++j) {
			os << matrix[i][j] << sep;
		}
		os << std::endl;
	}
	return os;
}
