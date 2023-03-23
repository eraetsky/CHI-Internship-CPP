#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include "Txt.h"

namespace l1
{
	size_t Txt::size() const
	{
		return this->lines;
	}

	size_t Txt::cnt_lines(std::istream &is)
	{
		if (!is.good()) return 0;
		std::string s;
		int cnt = 0;
		while (std::getline(is, s)) ++cnt;
		return cnt;
	}

	size_t Txt::cnt_lines(const char* filename)
	{
		std::ifstream is;
		is.open(filename, std::ios_base::in);
		return is.good() ? cnt_lines(is) : 0;
	}

	Txt::Txt(const char* filename) //constructor with 1 argument
	{
		std::ifstream is(filename);
		if (is)
		{
			is.seekg(0, std::ios_base::end);
			size_t buf = is.tellg();
			is.seekg(0);
			this->array = new char[buf + 1];
			this->lines = cnt_lines(filename);
			this->buf = strlen(this->array);
			is.read(this->array, buf);
			is.close();
		}
	}

	Txt::Txt(const Txt& t) : buf{ t.buf }, lines{ t.lines }  //copy constructor
	{
		if (!t.array) { this->array = nullptr; this->buf = 0; this->lines = 0; return; }
		if (this->array)
			delete[] this->array;
		this->array = new char[t.buf + 1];
		strncpy_s(this->array, this->buf + 1, t.array, t.buf);
	}

	Txt& Txt::operator= (const Txt& other) //copy = operator
	{
		if (!other.array) { Txt t; return t; }
		if (this == &other) { return *this; }
		if (this->array)
			delete[] this->array;
		this->array = new char[other.buf + 1];
		this->buf = other.buf;
		this->lines = other.lines;
		strncpy_s(this->array, this->buf + 1, other.array, other.buf);
		return *this;
	}

	Txt::Txt(Txt&& other) noexcept : array{ other.array }, buf{ other.buf }, lines{ other.lines } //move constructor
	{
		other.array = nullptr; other.buf = 0; other.lines = 0;
	}

	Txt& Txt::operator=(Txt&& other) noexcept // move = operator
	{
		if (this == &other) { return *this; }
		std::swap(this->array, other.array); 
		std::swap(this->buf, other.buf);
		std::swap(this->lines, other.lines);  
		return *this;
	}
	Txt::~Txt()
	{
		delete[] this->array;
	}

}

