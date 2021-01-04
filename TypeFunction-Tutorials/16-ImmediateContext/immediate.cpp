#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();
/*
    Thomas Kim's Definition for Immediate Context

    Immediate Context is the spot or the place in C++ .h or .cpp file, 
    where SFINAE, or "Substitution Failure Is Not An Error" works.

    In this broader sense of the Immediate Context,

    A. Immediate Context is used for overloading functions.
    
        A-1. More specifically, it is used to overload 
            function templates that return template types

        A-2. Immediate Context is also used for the functions
             with auto placeholder with trailing return type

    Immediate Context is in function template's template return type.
    Immediate Context is in the trailing return type
    specification of the function with auto return type placeholder.

    B. Immediate Context is also used for class template specializations.
   
    if we understand function object or functional object in C++
    C++ lambda is very simple and easy to understand

    In the same manner, if we understand Immediate Context and SFINAE,
    we can readily understand C++ concepts that will be introduced to C++20 Standard
*/

template<bool bEven=false>
struct is_even_st // <false>, when N is not even, 
{
    // we do not define type member here
};

template<>
struct is_even_st<true> // when N is even
{
    // when N is even, we define type member as below
    using type = int;
};

template<bool bEven>
using is_even_t = typename is_even_st<bEven>::type;

template<bool bOdd = false>
struct is_odd_st // <false>, N is not odd
{
    // when N is odd, we do not define type member
};

template<>
struct is_odd_st<true> // N is odd
{
    // when N is odd, we define type member as below
    using type = double;
};

template<bool bOdd>
using is_odd_t = typename is_odd_st<bOdd>::type;


template<int N>
is_even_t< N%2 == 0> // template return type, even case
even_or_odd()
{
    return 2*N;
}

template<int N>
is_odd_t< N%2 == 1> // template return type, odd case
 even_or_odd()
{
    return N*N; // N-squared
}

void test_even_or_odd()
{
    auto n_even = even_or_odd<4>();
    /*
        When we call even_or_odd<4>();
        C++ Compiler builds function overloading candidate list.
        Initially, C++ compiler finds the functions named even_or_odd(), 
        that matches function-call parameter list.
        In this case, C++ compiler builds
        template<int N> is_even_t< N%2 == 0> even_or_odd();
        template<int N> is_odd_t< N%2 == 1> even_or_odd();

        then C++ compiler substitutes 4 for N, then
        this is substitution stage
        template<4> is_even_t< 0 == 0> even_or_odd()
        template<4> is_odd_t< 0 == 1> even_or_odd()

        then 

        template<4> is_even_t< true > even_or_odd()
        template<4> is_odd_t< false> even_or_odd()

        then
        int even_or_odd() // valid function
        (return type is immediate context) even_or_odd() // this is invalid, substitution failed
        the failed function is removed from the function overloading candidates

        So, only  int even_or_odd() is left in the function overloading candidates list


    */
    
    auto n_odd = even_or_odd<5>();

    /*
        When the C++ compiler sees even_or_odd<5>() call,
        C++ compiler builds function overloading candidates list as below

        template<int N> is_even_t< N%2 == 0> even_or_odd();
        template<int N> is_odd_t< N%2 == 1> even_or_odd();

        then substitute 5 for template parameters

        s_even_t< 5%2 == 0> even_or_odd();
        is_odd_t< 5%2 == 1> even_or_odd();

        then

        is_even_t< 1 == 0> even_or_odd();
        is_odd_t< 1 == 1> even_or_odd();

        then

        is_even_t< false > even_or_odd();
        is_odd_t< true > even_or_odd();

        now,
        (empty) even_or_odd(); // invalid
        double even_or_odd(); // valid

        then 
        (empty) even_or_odd(); // invalid function overloading candidate is removed
        double even_or_odd(); // valid function overloading candidate is selecte for function call

        this place of the function template's template return type is Immediate Context
        when Substitution Fails, the corresponding
        function candidate is removed from the overloading candidate list
    */

    stream << "n_even = " << n_even <<
        ", type of n_even = " << Tpf_GetTypeCategory(n_even) << nl;

    stream << "n_odd = " << n_odd <<
        ", type of n_odd = " << Tpf_GetTypeCategory(n_odd) << nl;
}

// template<typename T>
// auto sum(T a, T b)
// {
//     return a + b;
// }

// template<typename S, typename T>
// auto sum(S a, T b)
// {
//     return a + b;
// }

// template<typename S, typename T>
// auto  // return type placeholder auto
// sum(S a, T b) -> decltype( a + b ) // -> decltype(expression) is called trailing return type specification
// {
//     return a + b;
// }

template<typename S, typename T>
auto sum(S a, T b) -> decltype( true ? a : b ) // this return's the common type of two arguments a and b
{                                               // only when a and b are operable
    return a + b;
}

/*
    when we want to test if the operation between two arguments are possible
    we can use true ? a : b expression, if a and b are operable,
    true ? a : b returns the common type of a and b
    if not operable, this function effectively SFINAE out.
*/
void test_trailing_return_type_specification()
{
    // auto r1 = sum(4, 5);
    // auto r1 = sum(4, 5.0);

    // will it work?
    auto r1 = sum(4, "what's up?");

    int a = 4;
    const char* b = "what's up";

    // what is the common type of int and a pointer?
    // we cannot get the common type of an int and a pointer
    // this feature turns out to be really useful
    // auto test = true ? a : b; 

    auto r2 = a + b; // int + const char* pointer
    // In C/C++, int can be added to or subtracted from pointers

    // if we want to prevent pointers and int arithmetic, we need to create a type function.
    // for this purpose, we will be using ternary operator ?:
    // by default, the ternary operator ?: returns the common type of the two arguments as below

    stream << "r1 = " << r1 << nl;
    stream << "r2 = " << r2 << nl;

    double d = 5.0;

    // this ternary operator ?: returns the common type of the two arguments, a and d
    auto r3 = true ? a : d; // int and double are operated, the resultant type is double

    // this is the mechanism C++ standard library type function std::common
    
    // But this std::common is, from my perspective, a misnomer.
    // it should have been named as std::is_operable.
    // We should not use std::common to get the common type of two arguments.
    // I will address this issue in my future sessions.
    // We SHOULD NEVER USE std::common TO EXTRACT the common type of two type arguments.

    // we are not interested in the return value of this ternary operator
    // we are interested in the type of this return value;

    // r3_t has the type of the ternary expression true ? a : d;
    // using r3_t = decltype(r3);

    // stream << "the type of r3_t ? " << Tpf_GetTypeName(r3_t) << nl;
  
    
}

int main()
{
    // test_even_or_odd();
    test_trailing_return_type_specification();
}