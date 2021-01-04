#include "pch.h"

#include <cstdio>

auto stream = tpf::stream();
auto nl = stream.get_console_out();

/*
    C++11 Standard was completely rewritten from scratch.
    Do not use C++98 Standard Practice in your C++ programming.
*/

std::string get_string(const char* msg)
{
    std::string str(msg);
    printf("The memory address of the string\'s internal buffer: %p\n", str.c_str());
    return str;
}

template<typename Type, typename... Types>
std::vector<Type> make_vector(Type&& first, Types&&... args)
{
    // it does not always work, but it's okay for this demonstration purpose
    std::vector<Type> v { std::forward<Type>(first), std::forward<Types>(args)... };
    printf("The memory address of the vector\'s internal buffer: %p\n", &v[0]);

    return v;
}

// if we want to achieve similar performance using C++98 Standard
void get_string(std::string& str, const char* msg)
{
    str.append(msg);
}

void test_modern_cpp()
{
    // the return value of get_string() is moved to 
    // str... that is,,, the memory allocated in the get_string() is recycled
    // or moved to str
    auto str = get_string("A big long long string literal to std::string ............. !!");
    printf("The memory address of the string\'s internal buffer: %p\n", str.c_str());

    // std::vector<int> is created once in make_vector(),
    // and the internal memory buffer is recycled and moved to
    // v... that is, std::vector<> is created only once in make_vector() and
    // recycled in this function...

    // the result is extreme performance that was not possible in C++98 Standard
    auto v = make_vector(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    printf("The memory address of the vector\'s internal buffer: %p\n", &v[0]);

    std::string c98_str;
    get_string(c98_str, "C98 style programming practice to achieve performance");

}

void const_char_pointer_reference()
{
    using literal_str_t = const char*;

    // literal string is stored in data section
    literal_str_t str1 = "I love it";

    stream << str1 << nl;

    // you are pointing str1 from one literal string to another literal string
    str1 = "You love me!";

    stream << str1 << nl;

    // you still cannot modify str1's elements.

    std::string real_string("This is real memory buffered, modifiable string");

    stream << real_string << nl;

    real_string[0] = 'S';

    stream << real_string << nl;

    // const char*& srf = str1;
    literal_str_t& srf = str1;

    srf = "another literal string";

    // we cannot do this
    //  srf[0] = 'A';

    stream << " srf = "<< srf << nl;


}

void tuple_basic()
{
    using tuple_t = std::tuple<int, double, const char*>;

    tuple_t t1 {1, 22.0/7.0, "This is so called tuple"};

    // we can access each element of tuple t
    
    stream << std::get<0>(t1) << nl;
    stream << std::get<1>(t1) << nl;
    stream << std::get<2>(t1) << nl;

   stream << t1 << nl;

    // std::cout  << t1 << std::endl;

    // C++17 Standard introduced "Structured Binding"
    auto& [age, weight, name] = t1;

    stream << "0-th element of t1: " << age << nl;
    stream << "1-th element of t1: " << weight << nl;
    stream << "2-th element of t1: " << name << nl;

    stream <<"tuple_t's element count: " << std::tuple_size_v<tuple_t> << nl;

    stream <<"The count of tuple_t's element: " << tft::type_count_v<tuple_t> << nl;

    // tuple t1's 0-th element type
    using t1_0_th_t = std::tuple_element_t<0, tuple_t>;
    using t1_1_th_t = std::tuple_element_t<1, tuple_t>;
    using t1_2_th_t = std::tuple_element_t<2, tuple_t>;
 

    stream << "t1's 0-th element type: " << Tpf_GetTypeName(t1_0_th_t) << nl;
    stream << "t1's 1-th element type: " << Tpf_GetTypeName(t1_1_th_t) << nl;
    stream << "t1's 2-th element type: " << Tpf_GetTypeName(t1_2_th_t) << nl;

    stream << t1 << nl;

    // std::make_tuple has a flaw in it.
    auto t2 = std::make_tuple(2, 3.4, "literal string");

    stream << t2 << nl;

    // std::cout << t2 << std::endl;

}

int main()
{
    // test_modern_cpp();

    // const_char_pointer_reference();

    tuple_basic();
}