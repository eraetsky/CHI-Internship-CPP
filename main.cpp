#include <iostream>
#include <string>
#include <vector>
#include <array>

class A 
{
    static int data;
};

class Date // example of class with own to_string method
{
    unsigned int dd;
    unsigned int mm;
    unsigned int yy;
public:
    Date(unsigned int d, unsigned int m, unsigned int y) : dd(d), mm(m), yy(y) {}
    std::string to_string() const
    {
        std::string s = "";
        if (dd < 10) { s += '0'; }
        s += std::to_string(dd) + '.';
        if (mm < 10) { s += '0'; }
        s += std::to_string(mm) + '.' + std::to_string(yy);
        return s;
    }
};

template<typename T>
struct has_to_string_method 
{
    static const bool value = false;
};

template<>
struct has_to_string_method<Date> 
{
    static const bool value = true;
};


template<typename T>    //need some comments here
struct has_const_iterator
{
private:
    typedef char yes;
    typedef struct { char array[2]; } no;

    template<typename C> static yes test(typename C::const_iterator*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template <typename T> //need some comments here
struct has_begin_end
{
    template<typename C> static char (&f(typename std::enable_if<
      std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
      typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&f(...))[2];

    template<typename C> static char (&g(typename std::enable_if<
      std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
      typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&g(...))[2];

    static bool const beg_value = sizeof(f<T>(0)) == 1;
    static bool const end_value = sizeof(g<T>(0)) == 1;
};

template<typename T>  //need some comments here
  struct is_container : std::integral_constant<bool, has_const_iterator<T>::value && has_begin_end<T>::beg_value && has_begin_end<T>::end_value> 
  {};

class CustomStringBuffer
{
    std::string buffer;
public:

    std::string ToString(const std::string& x) { return "S:" + x + " "; } //simple overloadig for std::string
    
    template<typename T>
    std::string ToString(const T& x,
        typename std::enable_if<has_to_string_method<T>::value, T>::type* = nullptr) //for classes with own to_string method
    {
        return "CTS:" + x.to_string() + " ";
    }

    template<typename T>
    std::string ToString(const T& x,
        typename std::enable_if<is_container<T>::value, T>::type* = nullptr) //for containers - std::array, std::vector, etc.
    {
        std::string res = "[";
        for (auto i : x)
            res += std::to_string(i) + ", ";
        res = res.substr(0,res.length() - 2) + "]";
        return "CNT:" + res + " ";
    }

    template<typename T>
    std::string ToString(T x,
        typename std::enable_if<std::is_same<decltype(x), bool>::value, T>::type* = nullptr) //for bool
    {
        std::string res = x ? "true" : "false";
        return "B:" + res + " ";
    }

    template<typename T>
    std::string ToString(T x,
        typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<decltype(x), bool>::value, T>::type* = nullptr) //for arithmetic types
    {
        return "A:" + std::to_string(x) + " ";
    }
    
    template<typename T>
    std::string ToString(const T& x,
        typename std::enable_if<std::is_empty<T>::value, T>::type* = nullptr) {return "";} //for empty classes

    template<typename T>
    std::string ToString(const T& x,
        typename std::enable_if<std::is_pointer<T>::value, T>::type* = nullptr) {return ToString(*x);} //for pointers to pod-types

    template<typename T>
    void operator += (const T& x){buffer += ToString(x);} 

    void print() { std::cout << buffer << std::endl; }
};

int main()
{
    Date d(2, 1, 2012); //object of class that has own to_string method
    A a; //empty object
    std::string s = "Hello!";
    int x = 100;
    int* intptr = &x;
    std::array<double,3> arr = {2.7, 5.6, 4.3};

    CustomStringBuffer csb;

    csb += d;
    csb += true;
    csb += 5;
    csb += 87.544;
    csb += 555688743256;
    csb += s;
    csb += a;
    csb += arr;
    csb += intptr;

    csb.print();
}

/*
CTS - convert to string template overload
A - arithmetic template overload
B - boolean template overload
S - string overload
CNT - standart container template overload

Output:
CTS:02.01.2012 B:true A:5 A:87.544000 A:555688743256 S:Hello! CNT:[2.700000, 5.600000, 4.300000] A:100
*/
