#include<iostream>
#include "Matrix.h"
#include "ConstexprMatrix.h"

//compile-time array from stackoverflow
template< int ... I > struct index_sequence {

	using type = index_sequence;
	using value_type = int;

	static constexpr std::size_t size()noexcept { return sizeof...(I); }
};

template< class I1, class I2> struct concat;

template< int ...I, int ...J>
struct concat< index_sequence<I...>, index_sequence<J...> >
	: index_sequence< I ..., (J + sizeof...(I))... > {};

template< int N > struct make_index_sequence_impl;

template< int N >
using make_index_sequence = typename make_index_sequence_impl<N>::type;

template< > struct make_index_sequence_impl<0> : index_sequence<> {};
template< > struct make_index_sequence_impl<1> : index_sequence<0> {};

template< int N > struct make_index_sequence_impl
	: concat< make_index_sequence<N / 2>, make_index_sequence<N - N / 2> > {};



template < class IS > struct mystruct_base;

template< int ... I >
struct mystruct_base< index_sequence< I ... > >
{

	static constexpr int array[]{ I ... };
};

template< int ... I >
constexpr int mystruct_base< index_sequence<I...> >::array[];

template< int N > struct mystruct
	: mystruct_base< make_index_sequence<N > >
{};

template< int ... I >
static constexpr std::array< int, sizeof...(I) >  build_array(index_sequence<I...>) noexcept
{
	return std::array<int, sizeof...(I) > { I... };
}
//

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
	std::cout << summ.getDim() << std::endl; //2

	constexpr std::array<int, 20> ma = build_array(make_index_sequence<20>{});
	for (auto e : ma) std::cout << e << ' ';
	std::cout << std::endl;

	constexpr matrix<double, 3, 3> constmatrix = {
	1., 2., 3.,
	4., 5., 6.,
	7., 8., 9. };

	//not working due to the lack of time and ideas about mars() implementation
	// see ConstexprMatrix.h
	//static_assert(rank(constmatrix) == 2, "rank"); 

	return 0;
}
