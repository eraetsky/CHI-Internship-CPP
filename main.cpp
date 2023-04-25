#include<iostream>
#include "Matrix.h"

bool custom_cmp(const int& a, const int& b)
{
	return a > b;
}

int main()
{
	Matrix<> m1(2, 3); //Matrix<int>
	m1[0][0] = 12;
	m1[1][0] = 42;
	m1[1][1] = 24;
	m1.resize(4, 4);
	m1[3][3] = 21;
	std::cout << m1[1][1] << std::endl; //24
	std::cout << m1 << std::endl;
	/*12; 0; 0; 0;
	42; 24; 0; 0;
	0; 0; 0; 0;
	0; 0; 0; 21;*/
	m1.sortAllRows(); //sort with default comparator
	m1.sortAllColumns(custom_cmp); 
	std::cout << m1 << std::endl;
	/*0; 0; 24; 42;
	0; 0; 0; 21;
	0; 0; 0; 12;
	0; 0; 0; 0;*/
	Matrix<int> m2(3, 3); m2[0][0] = 1; m2[2][0] = 4; m2[1][1] = 8;
	Matrix<int> sum = m1 + m2; //substracting likewise
	std::cout << sum << std::endl;
	/*1; 0; 24; 42;
	0; 8; 0; 21;
	4; 0; 0; 12;
	0; 0; 0; 0;*/
	Matrix <std::string> ms1(2, 2); ms1[0][0] = "Hello"; ms1[0][1] = "World";
	Matrix <std::string> ms2(2, 2); ms2[0][0] = ", "; ms2[0][1] = "!!!";
	Matrix <std::string> summ = ms1 + ms2;		//Hello,    World!!!
												//{empty}   {empty}
	std::cout << summ << std::endl;
	/*7 | 8 |
	  0 | 0 |*/
	std::cout << summ.getRank() << std::endl; //2
	return 0;
}
