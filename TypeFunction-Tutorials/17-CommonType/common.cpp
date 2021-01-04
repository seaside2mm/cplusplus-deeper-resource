#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

// this function does not work
template<typename S, typename T>
auto sum(S s, T t)
{
    // the problem is here
    auto the_less = s < t ? s : t;

    // the problem is here
    auto the_greater = s < t ? t : s;

    // we get the common type of S and T
    // the problem is here
    using result_t = std::common_type_t<S, T>;

    result_t r{};

    for(auto i = the_less; i < the_greater; ++i)
        r += i;

    return r;
}

void test_std_common()
{
    // it works
    short s1 = 1;
    unsigned short u1 = 11;
    auto r1 = sum(s1, u1);
    stream << "sum(" << Tpf_GetTypeCategory(s1) << ", " << Tpf_GetTypeCategory(u1) <<") = "
        << r1 << ", type of r1 = " << Tpf_GetTypeCategory(r1) << nl;

    // it works
    short s2 = -10;
    unsigned short u2 = 0;
    auto r2 = sum(s2, u2);
    stream << "sum(" << Tpf_GetTypeCategory(s2) << ", " << Tpf_GetTypeCategory(u2) <<") = "
        << r2 << ", type of r2 = " << Tpf_GetTypeCategory(r2) << nl;

    // it works, when int and unsigned int are positive
    int s3 = 1;
    unsigned int u3 = 11;
    auto r3 = sum(s3, u3);
    stream << "sum(" << Tpf_GetTypeCategory(s3) << ", " << Tpf_GetTypeCategory(u3) <<") = "
        << r3 << ", type of r3 = " << Tpf_GetTypeCategory(r3) << nl;

    // it failed, when int and unsigned int ... int s4 is negative
    int s4 = -10;
    unsigned int u4 = 0;
    auto r4 = sum(s4, u4);
    stream << "sum(" << Tpf_GetTypeCategory(s4) << ", " << Tpf_GetTypeCategory(u4) <<") = "
        << r4 << ", type of r4 = " << Tpf_GetTypeCategory(r4) << nl;


    // it works, when long long and unsigned long long are positive
    long long s5 = 1;
    unsigned long long u5 = 11;
    auto r5 = sum(s5, u5);
    stream << "sum(" << Tpf_GetTypeCategory(s5) << ", " << Tpf_GetTypeCategory(u5) <<") = "
        << r5 << ", type of r5 = " << Tpf_GetTypeCategory(r5) << nl;

    // it failed, when long long s6 is negative
    long long s6 = -10;
    unsigned long long u6 = 0;
    auto r6 = sum(s6, u6);
    stream << "sum(" << Tpf_GetTypeCategory(s6) << ", " << Tpf_GetTypeCategory(u6) <<") = "
        << r6 << ", type of r6 = " << Tpf_GetTypeCategory(r6) << nl;

}

// this function works correctly
template<typename S, typename T>
auto sum_fixed(S ss, T tt)
{
    // convert unsigned to signed
    std::make_signed_t<S> s = ss;

    // convert unsigned to signed
    std::make_signed_t<T> t = tt;

    // it is fixed, because signed and signed comparison
    auto the_less = s < t ? s : t;

    // it is fixed, because signed and signed comparison
    auto the_greater = s < t ? t : s;

    // we get the common type of S and T
    // we made the unsigned to signed common type
    using result_t = std::make_signed_t<  std::common_type_t<S, T>  >;

    result_t r{};

    for(auto i = the_less; i < the_greater; ++i)
        r += i;

    return r;
}

void test_fixed_std_common()
{
    // it works
    short s1 = 1;
    unsigned short u1 = 11;
    auto r1 = sum_fixed(s1, u1);
    stream << "sum_fixed(" << Tpf_GetTypeCategory(s1) << ", " << Tpf_GetTypeCategory(u1) <<") = "
        << r1 << ", type of r1 = " << Tpf_GetTypeCategory(r1) << nl;

    // it works
    short s2 = -10;
    unsigned short u2 = 0;
    auto r2 = sum_fixed(s2, u2);
    stream << "sum_fixed(" << Tpf_GetTypeCategory(s2) << ", " << Tpf_GetTypeCategory(u2) <<") = "
        << r2 << ", type of r2 = " << Tpf_GetTypeCategory(r2) << nl;

    // it works, when int and unsigned int are positive
    int s3 = 1;
    unsigned int u3 = 11;
    auto r3 = sum_fixed(s3, u3);
    stream << "sum_fixed(" << Tpf_GetTypeCategory(s3) << ", " << Tpf_GetTypeCategory(u3) <<") = "
        << r3 << ", type of r3 = " << Tpf_GetTypeCategory(r3) << nl;

    // it failed, when int and unsigned int ... int s4 is negative
    int s4 = -10;
    unsigned int u4 = 0;
    auto r4 = sum_fixed(s4, u4);
    stream << "sum_fixed(" << Tpf_GetTypeCategory(s4) << ", " << Tpf_GetTypeCategory(u4) <<") = "
        << r4 << ", type of r4 = " << Tpf_GetTypeCategory(r4) << nl;


    // it works, when long long and unsigned long long are positive
    long long s5 = 1;
    unsigned long long u5 = 11;
    auto r5 = sum_fixed(s5, u5);
    stream << "sum_fixed(" << Tpf_GetTypeCategory(s5) << ", " << Tpf_GetTypeCategory(u5) <<") = "
        << r5 << ", type of r5 = " << Tpf_GetTypeCategory(r5) << nl;

    // it failed, when long long s6 is negative
    long long s6 = -10;
    unsigned long long u6 = 0;
    auto r6 = sum_fixed(s6, u6);
    stream << "sum_fixed(" << Tpf_GetTypeCategory(s6) << ", " << Tpf_GetTypeCategory(u6) <<") = "
        << r6 << ", type of r6 = " << Tpf_GetTypeCategory(r6) << nl;

}

/*
    std::common_type_t<S, T> is based on true ? S() : T(), the ternary operator ?:
    true ? S() : T() this ternary operator returns the common type of S and T

    it does not work for unsigned and signed int.
    For signed int and unsigned int, this ternary operator ?: returns unsigned int
    For long int or long long int and unsigned long or unsigned long long,
    this ternay operator ?: returns unsigned long, unsigned long long respectively.

    This ternary operator ?: is not adequate for operation with signed long and signed long long
    with unsigned long and unsigned long long respectively.
*/

// template<typename S, typename T>
// using signed_common_t = std::make_signed_t<std::common_type_t<S, T>>;

// We cannot use std::common_type_t<> without understanding this type function correctly

/*
    The reason C++ language is hard to learn is that
    we have to understand down to the bottom-level or low-level programming
    as well as highest-level template metaprogramming.
*/

template<typename S, typename T, typename... Types>
using signed_common_t = std::make_signed_t<std::common_type_t<S, T, Types...>>;

// this function works correctly
template<typename S, typename T>
auto sum_fixed_again(S ss, T tt)
{
    // convert unsigned to signed
    std::make_signed_t<S> s = ss;

    // convert unsigned to signed
    std::make_signed_t<T> t = tt;

    // it is fixed, because signed and signed comparison
    auto the_less = s < t ? s : t;

    // it is fixed, because signed and signed comparison
    auto the_greater = s < t ? t : s;

    // we get the common type of S and T
    // we made the unsigned to signed common type
    using result_t = signed_common_t<S, T>;

    result_t r{};

    for(auto i = the_less; i < the_greater; ++i)
        r += i;

    return r;
}

void test_make_signed_common()
{
    // it works
    short s1 = 1;
    unsigned short u1 = 11;
    auto r1 = sum_fixed_again(s1, u1);
    stream << "sum_fixed_again(" << Tpf_GetTypeCategory(s1) << ", " << Tpf_GetTypeCategory(u1) <<") = "
        << r1 << ", type of r1 = " << Tpf_GetTypeCategory(r1) << nl;

    // it works
    short s2 = -10;
    unsigned short u2 = 0;
    auto r2 = sum_fixed_again(s2, u2);
    stream << "sum_fixed_again(" << Tpf_GetTypeCategory(s2) << ", " << Tpf_GetTypeCategory(u2) <<") = "
        << r2 << ", type of r2 = " << Tpf_GetTypeCategory(r2) << nl;

    // it works, when int and unsigned int are positive
    int s3 = 1;
    unsigned int u3 = 11;
    auto r3 = sum_fixed_again(s3, u3);
    stream << "sum_fixed_again(" << Tpf_GetTypeCategory(s3) << ", " << Tpf_GetTypeCategory(u3) <<") = "
        << r3 << ", type of r3 = " << Tpf_GetTypeCategory(r3) << nl;

    // it failed, when int and unsigned int ... int s4 is negative
    int s4 = -10;
    unsigned int u4 = 0;
    auto r4 = sum_fixed_again(s4, u4);
    stream << "sum_fixed_again(" << Tpf_GetTypeCategory(s4) << ", " << Tpf_GetTypeCategory(u4) <<") = "
        << r4 << ", type of r4 = " << Tpf_GetTypeCategory(r4) << nl;


    // it works, when long long and unsigned long long are positive
    long long s5 = 1;
    unsigned long long u5 = 11;
    auto r5 = sum_fixed_again(s5, u5);
    stream << "sum_fixed_again(" << Tpf_GetTypeCategory(s5) << ", " << Tpf_GetTypeCategory(u5) <<") = "
        << r5 << ", type of r5 = " << Tpf_GetTypeCategory(r5) << nl;

    // it failed, when long long s6 is negative
    long long s6 = -10;
    unsigned long long u6 = 0;
    auto r6 = sum_fixed_again(s6, u6);
    stream << "sum_fixed_again(" << Tpf_GetTypeCategory(s6) << ", " << Tpf_GetTypeCategory(u6) <<") = "
        << r6 << ", type of r6 = " << Tpf_GetTypeCategory(r6) << nl;

}

int main()
{
    // test_std_common();
    // test_fixed_std_common();
    test_make_signed_common();
}