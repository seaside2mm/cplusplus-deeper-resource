#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

/*
    First off, let's think about the problem solving strategy.

    When we face a new problem in real life, we first have to 
    understand the problem at hand. After having a solid understanding
    of the problem, we analyze the problem and seek for its solution.

    My problem at hand is to help you understand this complex technology
    in a simple and managebale manner. I have thought a few days about
    how I can explain this complex technology to my audience.

    And I decided that by lowering my eyes... level to those of my audience,
    I thought I can deliver this complex technology to new C++ learners.

    Let's peep into the problem that C++98 language architects had.
    By understanding the problem from the perspective of those C++ language architects,
    we can better understand this monster and apply it to our daily practice.

    Speaking from my personal opinion, this technology is really a monster.
    The best achievement in the history of Computer Science.

    This journey started after since C++98. Numerous attempts have been made.
    After 13 years of experiments, it finally arrived in C++11, 2011.

    But still, this technology was incomplete as of C++11 Standard.
    By introducing decltype(auto) to C++14, and by introducing if constexpr
    to C++17, this technology is finally incarnated into Modern C++ Language in 2017.

    So, this technology took almost 18 years to arrive at its final destination.
    
    To harness this monster and put it at your service, you have to pay its due price.
    If ready, brace yourself. I will do my best help you tame this monster.
*/

// this is C++98 compiler calculation method
// each time this function is called,
// type int is created twice.
int c98_sum(const int& a, const int& b)
{
    int r = a + b; // int r is created

    return r; // C++98 compiler creates an int internally, then copy r to this internal variable of type int.
}

// this is C++11 compiler caluclation method
// this function does NOT create int even once
// it never creates int.
const int& c11_sum(int& r, const int& a, const int& b)
{
    r = a + b;

    return r;
}

/*
    I need to remind you of 3 things.

    1. int&& - is recyclable, 
            // and we have to typecast to int&& 
            // if we want to recycle non const non reference local variable type
            // and also rvalue reference type int&&

    2. if we pass non const non reference return value to a function
       that return value is pass as int&& - rvalue reference.

    3. In C++11... the local variables created in an expression
        survive until that expression ends...
        this is C++11 promise or contract to recycle rvalues.
*/

void func(const char* msg, const int& a)
{
    stream <<msg << " - func(const int& a) called "<< nl;
}

void func(const char* msg, int&& a)
{
    stream <<msg << " - func(int&& a) called "<< nl;
}

int get_int() { return 1; }

void test_call_parameter_rvalues()
{
    int a = 5;
    int&& r = 6;
    int& l = a;

    func("int a - ", a);
    func("int&& r", r);
    func("int& l", l);
    func("int 5 - ", 5);
    func("get_int() - ", get_int());
    func("(int&&)a ", (int&&)a);
    func("(int&&)r", (int&&)r);
}

void understand_basic()
{
    int a = 1, b = 2, c = 3;

    int cpp = a + b + c;

    // C++98 compiler calculation method
    // for c98 = a + b + c expression
    // in total, int is created 5 times in C++98 compiler
    int c98 = c98_sum( c98_sum(a, b), c);

    // C++11 compiler calculation method
    // for c11 = a + b + c expression
    // this method creates int only once for c11
    int c11; 
    c11_sum(c11, c11_sum(c11, a, b), c);

    stream << "cpp = " << cpp << nl;
    stream << "c98 = " << c98 << nl;
    stream << "c11 = " << c11 << nl;
    
}

int sum(const int& a, const int& b)
{
    return a + b; // one int is created
}

int&& sum(int&& r, const int& b)
{
    r += b; // no int was created, r is recycled

    return (int&&)r;
}

int&& sum(const int& a, int&& r)
{
    r += a; // no int was created, r is recycled

    return (int&&)r;
}

int&& sum(int&& a, int&& b)
{
    a += b; // no int was created, a is recycled

    return (int&&)a;
}

void test_modern_cpp()
{
    int a = 1, b = 2, c = 3;

    // the return value of sum(a, b) is recycled
    // sum( sum(a, b), c), its return value
    // is recyled and moved to cpp
    // so... 

    // local variable created in sum(a, b) call
    // is moved to cpp.

    // the question is how this local variable returned from sum(a, b)
    // survives until cpp = sum(sum(a, b), c) ends ?
    // that is the promise of C++11.
    int cpp = sum( sum(a, b), c);

    stream << "cpp = " << cpp << nl;
}

using flat_t = int;  // non const non reference type
using necyclable_t = const int&; // non recyclable type
using recyclable_t = int&&; // recyclable type

flat_t formula(necyclable_t a, necyclable_t b)
{
    return a * b; // operation
}

recyclable_t formula(recyclable_t r, necyclable_t n)
{
    r *= n; return (recyclable_t)r;
}

recyclable_t formula(necyclable_t n, recyclable_t r)
{
    r *= n; return (recyclable_t)r;
}

recyclable_t formula(recyclable_t a, recyclable_t b)
{
    a *= b; return (recyclable_t)a;
}

void test_formula()
{
    int a = 2, b = 3, c = 4;

    int d = formula(formula(a, b), c);

    stream << "d = a * b * c = " << d << nl;
}

template<typename S, typename T>
decltype(auto) product(S&& s, T&& t)
{
    if constexpr( std::is_rvalue_reference_v<decltype(s)> )
    {
        s *= t;
        return std::forward<S>(s);
    }
    else if constexpr( std::is_rvalue_reference_v<decltype(t)> )
    {
        t *= s;
        return std::forward<T>(t);
    }
    else 
    {
        // only one possibility
        // both s and t are not rvalue reference
        auto r = s * t; // one local variable is created
                        // this local variable will be recycled
        return r;
    }
}


void test_product()
{
    int a = 2, b = 3, c = 4, d = 5;

    int fact_5 = product(product(product(a, b), c), d);

    stream << "fact_5 = " << fact_5 << nl;
}
int main()
{
      // understand_basic();
     // test_call_parameter_rvalues();
     // test_modern_cpp();

     // test_formula();

     test_product();
}