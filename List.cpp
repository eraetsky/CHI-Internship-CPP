#include "List.h"
#include <iostream>

template<typename T, size_t N>
List<T, N>::List() : array{ new T[N] }, cnt_elements{ 0 } {}

template<typename T, size_t N>
List<T, N>::~List() { delete[] this->array; this->array = nullptr; cnt_elements = 0; }

template<typename T, size_t N>
size_t List<T,N>::size() const
{
	return this->cnt_elements;
}

template<typename T, size_t N>
const T& List<T, N>::operator [] (int index) const
{
	return (this->array)[index];
}

template<typename T, size_t N>
void List<T, N>::operator += (const T& value)
{
	if (this->cnt_elements < N)
	{
		this->array[cnt_elements] = value;
		++cnt_elements;
	}
	else
		std::cerr << "Failed to push: not enough memory.\n";
}

template<typename T, size_t N>
void List<T, N>::operator += (T&& value)
{
	if (this->cnt_elements < N)
	{
		this->array[cnt_elements] = std::move(value);
		++cnt_elements;
	}
	else
		std::cerr << "Failed to push: not enough memory.\n";
}