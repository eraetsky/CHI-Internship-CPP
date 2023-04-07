#include <iostream>
#include <string>

class Date // example of class with user to_string method
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
struct has_to_string_method<Date> {
    static const bool value = true;
};


class CustomStringBuffer
{
    std::string buffer = "";
public:
  
 
    std::string ToString(const std::string& x) { return "CTS:" + x + " "; } //for std::string

    template<typename T>
    std::string ToString(const T& x,
        typename std::enable_if<has_to_string_method<T>::value, T>::type* = 0) //for classes with to_string method
    {
        return "CTS:" + x.to_string() + " ";
    }

    template<typename T>
    std::string ToString(T x,
        typename std::enable_if<std::is_same<decltype(x), bool>::value, T>::type* = 0) //for bool
    {
        std::string res = x ? "true" : "false";
        return "B:" + res + " ";
    }

    template<typename T>
    std::string ToString(T x,
        typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<decltype(x), bool>::value, T>::type* = 0) //for arithmetic types
    {
        return "A:" + std::to_string(x) + " ";
    }
 
    template<typename T>
    void operator += (const T& x){buffer += ToString(x);}

    void print() { std::cout << buffer << std::endl; }
};

int main()
{
    Date d(2, 1, 2012);
    std::string s = "Hello!";
    CustomStringBuffer csb;
    csb += d;
    csb += true;
    csb += 5;
    csb += false;
    csb += 555688743256;
    csb += s;
    csb.print();
   
 
}
