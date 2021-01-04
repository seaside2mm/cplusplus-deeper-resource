#include "pch.h"
#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

/*
    First off, we will learn how to use std::forward.
*/


// a function that takes a rvalue reference as parameter
void get_rvalue(int&& v)
{
    stream << "get_rvalue() called" << nl;
}

// the return value of type int, or non reference type is called 
// rvalue... so return_int returns an rvalue
// we can pass rvalue to a function that takes rvalue reference
int return_int() { return 1; }

// int&& as return value category is xvalue
// we can also pass xvalue or rvalue reference return value 
// type to a function that takes rvalue reference
// int&& ... is called xvalue
int&& return_xvalue(int a)
{
    return (int&&) a;
}

// int as the function return value type is called "pure rvalue"
// int&& as the function return value type is called "xvalue" or eXpiring value


void rvalue_basic()
{
    int a = 6;
    
    int& l = a;

    // r is a rvalue reference
    int&& r = 5;

    // 1. since a is of type int, a's value category is lvalue
    // we cannot pass lvalue to a function that takes rvalue reference
    // get_rvalue( a );

    // 2. since l is lvalue reference
    // we cannot pass lvalue reference to a function that takes rvalue reference
    // get_rvalue( l );

    // 3. the type of r is rvalue reference
    // we cannot pass rvalue reference to a function that takes rvalue reference as its parameter
    // get_rvalue( r );

    // 4. we can pass pure rvalue to a function that takes rvalue reference
    get_rvalue( 5 );

    // 5. we can pass the rvalue return value to a function that takes rvalue reference
    get_rvalue( return_int() );

    // 6. we can pass the xvalue (or int&& as return type) to a function that takes rvalue reference
    get_rvalue( return_xvalue(5) );

}

void rvalue_fundamentals()
{
    int a = 6;
    
    int& l = a;

    // r is a rvalue reference
    int&& r = 5;

    // if we want to pass lvalue to a function that takes rvalue reference
    // we have to typecast to rvalue reference
    get_rvalue( (int&&) a );
    get_rvalue( (int&&) l );
    get_rvalue( (int&&) r );

}

void rvalue_fundamentals_move()
{
    int a = 6;
    
    int& l = a;

    // r is a rvalue reference
    int&& r = 5;

    // if we want to pass lvalue to a function that takes rvalue reference
    // we have to typecast to rvalue reference

    // std::move is a typecast to rvalue reference
    get_rvalue( std::move(a) );
    get_rvalue( std::move(l) );
    get_rvalue( std::move(r) );

}


void rvalue_fundamentals_forward()
{
    int a = 6;
    
    int& l = a;

    // r is a rvalue reference
    int&& r = 5;

    // if we want to pass lvalue to a function that takes rvalue reference
    // we have to typecast to rvalue reference

    // std::move is a typecast to rvalue reference
    
    // std::forward<int>(a) is equivalent to (int&&)a
    get_rvalue( std::forward<int>(a) ); 

    // std::forward<int>(l) is equivalent to (int&&)l
    get_rvalue( std::forward<int>(l) );


    // both std::move() and std::forward<>() are typecast to rvalue reference
    // std::move() blindly typecast to rvalue reference
    // whereas std::forward<>() is smarter typecast to rvalue reference

    // std::forward<int&>(l) is equivalent to (int& &&)l == (int&)l
    // get_rvalue( std::forward<int&>(l) );
    
    // std::forward<int>(r) is equivalent to (int&&)r 
    get_rvalue( std::forward<int>(r) );

    // std::forward<int&&>(r) is equivalent to (int&& &&)r = (int&&)r
    get_rvalue( std::forward<int&&>(r) );
    
}

// both std::move and std::forward are typecast to rvalue reference
// std::move forces typecast to rvalue reference, so it always succeeds
// std::forward<>() is also a typecast. but it can fail because it is a smarter typecast

// std::forward<>() is meant to be used together with
// forwarding reference (or is called universal reference)
// in my next session, we will learn std::forward<>>() together with
// forwarding reference

int main()
{
    rvalue_basic();
    // rvalue_fundamentals();
    // rvalue_fundamentals_move();
    // rvalue_fundamentals_forward();
    
}