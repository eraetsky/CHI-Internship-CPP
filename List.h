#pragma once
template<typename T, size_t N = 0>
class List
{
private:
	T* array = nullptr;
	size_t cnt_elements = 0;
public:
	List();
	~List();
	size_t size() const;
	const T& operator [] (int index) const;
	void operator += (const T&);
	void operator += (T&&);
};