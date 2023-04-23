#pragma once
#include<iostream>
#include <string>
#include <vector>

//used my CustomStringBuffer from previous task

template<typename T>
struct has_const_iterator
{
private:
    typedef char yes;
    typedef struct { char array[2]; } no;  //in order to sizeof(no) != sizeof(yes)

    template<typename C> static yes test(typename C::const_iterator*);
    template<typename C> static no  test(...); //if the function in the prev line doesn't match, this is selected
public:
    static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
    typedef T type;
};

template <typename T>
struct has_begin_end
{
    template<typename C> static char(&f(typename std::enable_if<
        std::is_same<decltype(static_cast<typename C::const_iterator(C::*)() const>(&C::begin)),
        typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char(&f(...))[2];

    template<typename C> static char(&g(typename std::enable_if<
        std::is_same<decltype(static_cast<typename C::const_iterator(C::*)() const>(&C::end)),
        typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char(&g(...))[2];

    static bool const beg_value = sizeof(f<T>(nullptr)) == 1;
    static bool const end_value = sizeof(g<T>(nullptr)) == 1;
};

template<typename T>  //for std containers with 
struct is_container : std::integral_constant<bool, has_const_iterator<T>::value&& has_begin_end<T>::beg_value&& has_begin_end<T>::end_value>
{};

class CustomStringBuffer
{
    std::string buffer;
public:

    std::string ToString(const std::string& x) { return x + " "; } //simple overloadig for std::string


    template<typename T>
    std::string ToString(const T& x,
        typename std::enable_if<is_container<T>::value, T>::type* = nullptr) //for containers - std::array, std::vector, etc.
    {
        std::string res = "[";
        for (auto it = x.begin(); it != x.end(); ++it)
            res += std::to_string(*it) + ", ";
        res = res.substr(0, res.length() - 2) + "]";
        return res + " ";
    }

    template<typename T>
    std::string ToString(T x,
        typename std::enable_if<std::is_same<decltype(x), bool>::value, T>::type* = nullptr) //for bool
    {
        std::string res = x ? "true" : "false";
        return  res + " ";
    }

    template<typename T>
    std::string ToString(T x,
        typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<decltype(x), bool>::value, T>::type* = nullptr) //for arithmetic types
    {
        return  std::to_string(x) + " ";
    }

    template<typename T>
    std::string ToString(const T& x,
        typename std::enable_if<std::is_empty<T>::value, T>::type* = nullptr) {
        return "";
    } //for empty classes

    template<typename T>
    std::string ToString(const T& x,
        typename std::enable_if<std::is_pointer<T>::value, T>::type* = nullptr) {
        return ToString(*x);
    } //for pointers to pod-types

    template<typename T>
    void operator += (const T& x) { buffer += ToString(x); }

    void print(std::ostream& out) { out << buffer << std::endl; }
};
