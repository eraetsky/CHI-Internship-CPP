#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <ostream>

using uint = unsigned int;

template<typename T>
bool default_cmp(const T& a, const T& b)
{
	return a < b;
}

template<>
bool default_cmp(const std::string& a, const std::string& b) 
{
	return a.length() < b.length();
}


template <typename T>
class Row
{
private:
	size_t ncols;
	std::vector<T> r;
public:
	Row() = default;
	Row(size_t n) : ncols(n), r(std::vector<T>(n)) {}
	T& operator[] (uint);
	const T& operator[] (uint) const;
	void resize(size_t);
	void sortRow(bool (*cmp)(const T& a, const T& b) = default_cmp);
	void fill(const T&);
};

template <typename T>
T& Row<T>::operator[] (uint ind)
{
	try {
		if (ind >= ncols) throw ("Exception: Index out of range");
		return this->r[ind];

	}
	catch (const char* err) { std::cerr << err << std::endl; }
}

template <typename T>
const T& Row<T>::operator[] (uint ind) const
{
	try {
		if (ind >= ncols) throw ("Exception: Index out of range");
		return this->r[ind];

	}
	catch (const char* err) { std::cerr << err << std::endl; }
}

template<typename T>
void Row<T>::resize(size_t d)
{
	try {
		if (d <= this->ncols) throw("Exception: Possible lose of data while resizing matrix");
		else
		{
			this->r.resize(d);
			this->ncols = d;
		}
	}
	catch (const char* err) { std::cerr << err << std::endl; }
}

template<typename T>
void Row<T>::sortRow(bool (*cmp)(const T& a, const T& b))
{
	std::sort(this->r.begin(), this->r.end(), cmp);
}

template<typename T>
void Row<T>::fill(const T& value)
{
	std::fill(this->r.begin(), this->r.end(), value);
}