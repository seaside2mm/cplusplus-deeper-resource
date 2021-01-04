#include "pch.h"
#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

/*
    In next session, we will be using 
    
    (1) variadic template parameter
    (2) initializer list
    (3) class template argument deduction guide
    (4) and we will learn advanced type functions.
    
*/

/*
    In modern C++, there are two syntax for initialization.
    One is using parentheses (), another is using curly brace { }

    It is crucial to understand the difference between () and {}
*/

// An aggregate type should not have custom constructor
// An aggregate type should not have virtual member function
// An aggregate type can have non-virtual member function
// All data fields in an aggregate type should be in public: section
// we cannot make template class argument deduction guide for aggregate types
// because aggregate types do not have custom constructor
template<typename T>
class AggregateType
{
    public:
        T m_value; // this should in public section if AggregateType<T> wants to be an aggregate type

        // we cannot provide custom constructor, but can have non-virtual and friend functions

    template<typename CharType>
    friend decltype(auto) operator<<(tpf::stream_t<CharType>& os, const AggregateType& agr)
    {
        os << agr.m_value; return os;
    }

};

void test_aggregate_type()
{
    // when we initialize an aggregate type, we should use curly brace {}
    // we cannot use parenthese () to initialize aggregate types
    AggregateType<int> a{5};

    stream << a << nl;

    // because aggregate types do not have custom constructor(s)
    // we cannot use parentheses () to initialize aggregate types
    // AggregateType<int> b(6);
    // stream << b << nl;
}

// non aggregate type is conventional class
// it can have virtual member function,
// can have custom constructor
// data members can be private
//
// for non-aggregate types, with constuctor that does not take an instance of
// std::initializer_list, we can use both parentheses () and curly brace {} syntax
// to initialize data member field through the constructor
template<typename T>
class NonAggregateType
{
    private:
        T m_value{};

    public:

        // tied constructor without using std::initializer
        NonAggregateType(T value): m_value{ value }
        {
            stream << "constructor without using initializer called " << nl;
        }

        // tied constructor with std::initializer_list
        NonAggregateType(std::initializer_list<T> lst) // do not use reference initializer_list for constructor
        {
             stream << "constructor using initializer_list called " << nl;
             m_value = *std::begin(lst);
        }

    template<typename CharType>
    friend decltype(auto) operator<<(tpf::stream_t<CharType>& os, const NonAggregateType& nag)
    {
        os << nag.m_value; return os;
    }
};

void test_non_aggregate_type()
{
    // When NonAggregateType has a constructor that does not take an instance of std::initializer_list
    // we can use both parenthese () syntax and curly brace syntax
    NonAggregateType n1(1);

    stream << "n1 = " << n1 << nl;

    NonAggregateType n2{3.5};

    stream << "n2 = " << n2 << nl;

    // when a constructor with initializer_list is provided
    // curly brace {} syntax uses the initializer_list constructor
    //
    // if a constructor with initializer_list is not provided
    // then curly brace {} syntax uses conventional constructor ()
}

int main()
{
    // test_aggregate_type();
    test_non_aggregate_type();
}