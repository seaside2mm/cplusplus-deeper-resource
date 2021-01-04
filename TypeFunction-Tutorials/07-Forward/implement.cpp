#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void func(const char* msg, int& a)
{
    stream << msg << " - int& a" << nl;
}

void func(const char* msg, int&& a)
{
    stream << msg << " - int&& a" << nl;
}

/*
    In C++, when a parameter is passed to another function
    the type of the parameter gets distorted.
*/

void test_parameter_distortion()
{
    int v = 5;
        func("int v = 5 - ", v);

    int& l = v;
        func("int& l = v - ", l);

    int&& r = 5;
        func("int&& r = 5 - ", r);
}

/*
    the purpose of std::forward<>() is to prevent
    the type distortion of the parameter while passing from one function to another
    so, it is named forward - to forward the parameter to another function without type distortion
*/
void test_std_forward()
{
    int v = 5;
        func("std::forward<int>(v) - ", std::forward<int>(v));
        func("std::forward<int&&>(v) - ", std::forward<int&&>(v));
    
    int& l = v;
        func("std::forward<int&>(l)", std::forward<int&>(l));

    int&& r = 5;
        func("std::forward<int&&>(r) - ", std::forward<int&&>(r));
        func("std::forward<int>(r) - ", std::forward<int>(r));
}

// this is primary template
template<typename T> // primary template determines the count of parameter
struct remove_reference_st // T is neither lvalue reference nor rvalue reference
{
    using type = T;
};

// partial specialization
template<typename T>
struct remove_reference_st<T&> // when the parameter is lvalue reference, T& - T == int, T& = int&
{
    using type = T;
};

template<typename T>
struct remove_reference_st<T&&> // when the parameter is rvalue reference, T&& - T == int, T&& = int&&
{
    using type = T;
};

template<typename T>
using remove_reference_t = typename remove_reference_st<T>::type;

// non-deduced context
// template<typename T>
// T&& forward( typename remove_reference_st<T>::type& v)

/*
    In C++ Standard, a few functions are marked with keyword noexcept
    functions marked with noexcept should not leak exceptions
    if those functions marked with noexcept leak an exception,
    then the program crashes. For more about keyword noexcept,
    please refer to https://goo.gl/Y97uJy
*/
template<typename T>
T&& forward( remove_reference_t<T>& v) noexcept // &v is for lvalue reference
{
   return static_cast<T&&>(v);
}

template<typename T>
T&& forward( remove_reference_t<T>&& v) noexcept // &&v is for rvalue reference
{
   return static_cast<T&&>(v);
}

void test_forward()
{
    int a = 5;
    int&& r = 6;

    forward<int>(a);
    forward<int>(r);
    forward<int&&>(5); // call forward with pure rvalue
                        // it does not work because we have not 
                        // yet defined &&v rvalue reference parameter of forward()

}
void test_remove_reference()
{
    int a = 5;
        stream << "type of a = " << Tpf_GetTypeCategory(a) << nl;
        stream << "type of remove_reference_t<int> = " 
            << Tpf_GetTypeName( remove_reference_t<int> ) << nl;
    
    int& l = a;
        stream << "type of l = " << Tpf_GetTypeCategory(l) << nl;
        stream << "type of remove_reference_t<int&> = " 
            << Tpf_GetTypeName( remove_reference_t<int&> ) << nl;
    
    int&& r = 5;
        stream << "type of r = " << Tpf_GetTypeCategory(r) << nl;
        stream << "type of remove_reference_t<int&&> = " 
            << Tpf_GetTypeName( remove_reference_t<int&&> ) << nl;
}

/*
    non-deduced context is what std::forward<>() makes difference from std::move()

    Both std::forward<>() and std::move() are typecast to rvalue reference.
    But std::move() blindly typecasts to rvalue reference
    whereas std::forward<>() typecasts to revalue reference SMARTLY.
*/

// primary template
template<typename T>
struct dummy_struct; // primary template determines the parameter count

template<>
struct dummy_struct<double> // T == double
{
    using type = int;
};

template<>
struct dummy_struct<int> // T == int
{
    using type = int;
};

template<typename T>
using dummy_t = typename dummy_struct<T>::type;

// this is so called "non-deduced context"
// C++ compiler cannot determine the type of the template parameter for T
//
// this non-deduced context plays an important role for std::forward

// template<typename T>
// void call_non_deduced_context_func( typename dummy_struct<T>::type& d)

template<typename T>
void call_non_deduced_context_func( dummy_t<T>& d)
{
    stream << "call_non_deduced_context_func() is called " << nl;
}

void test_non_deduced_context()
{
    int a = 5;
    int&& r = 5;

    call_non_deduced_context_func<int>(a);
    call_non_deduced_context_func<double>(r);
}

// for your refernce

template<typename T> // this template parameter T
void func_param(const T&& v) // in this function-call parameter, T and v are not forwarding reference
{ }                          // if template parameter is T
                             // the function-call parameter type should be T&&
                             // const T&& v is not universal or forwarding reference

template<typename T>
struct my_struct
{
    static void function(T&& v) // this T&& is not forwarding reference
    {                            // because this T is not the template parameter for function()
                                // this T is the template parameter for the struct my_struct
    }

    template<typename S>    // the template parameter type for okay_forwarding_reference()
    static void okay_forwarding_reference(S&& v) // here, S&& is forwarding reference
    {                                            // because the template parameter type is S

    }
};

template<typename T>
void the_forwarded(const char* msg, T&& v)
{
    stream << " - the_forwarded - " << Tpf_GetTypeCategory(v) << nl;
}

template<typename T> // the template parameter T
void the_forwarder(const char* msg, T&& v) // the function-call parameter v
{
    /*
        local int --- T: int&,          v: int&
        int&      --- T: int&,          v: int&
        int&&     --- T: int&,          v: int&
        prvalue 5 --- T: int,           v: int&&
        return 8  --- T: int,           v: int&&

        Now, I am talking about Reference Collapsing Rule
        
        C++ Standard says that for any type T
        T&  + && => T&,  static_cast<int& &&>(v)  => static_cast<int&>(v)
        T&& + &  => T&,  static_cast<int&& &>(v)  => static_cast<int&)(v)
        T&& + && => T&&, static_cast<int&& &&>(v) => static_cast<int&&>(v)
        T& + &   => T&,  static_cast<int& &>(v)   => static_cast<int&>(v)

        Regard & as 0, && as 1

        &  * && == 0 * 1 = 0 => &
        && * &  == 1 * 0 = 0 => &
        && * && == 1 * 1 = 1 => &&
        & * &   == 0 * 0 = 0 => &

        These are that famous "Reference Collapsing Rule"

        template<typename T>
        T&& forward ///
        {
            return static_cast<T&&>(v);
        }
                                        forward: return static_cast<T&&>(v);
        -------------------------------------------------------------------------------
        local int --- T: int&,          static_cast<int& &&>(v) => static_cast<int&>(v)  - lvalue reference
        int&      --- T: int&,          static_cast<int& &&>(v) => static_cast<int&)(v)  - lvalue reference
        int&&     --- T: int&,          static_cast<int& &&>(v) => static_cast<int&)(v)  - lvalue reference
        prvalue 5 --- T: int,           static_cast<int  &&>(v) => static_cast<int&&)(v) - rvalue reference
        return 8  --- T: int,           static_cast<int  &&>(v) => static_cast<int&&)(v) - revalyue referece

    */
    stream << msg <<" - type of T: " << Tpf_GetTypeName(T)
        << ", type of v: " << Tpf_GetTypeCategory(v) << " --- ";

    stream << "std::forward<T>(v) => " << Tpf_GetTypeCategory( std::forward<T>(v) ) <<", ";

    // we are not applying std::forward
    // the_forwarded(msg, v);

    // we are now forwarding
    the_forwarded(msg, std::forward<T>(v));
}

int return_prvalue() { return 8; }

void the_caller()
{
    int a = 5;
        the_forwarder("int a = 5", a);

    int& l = a;
        the_forwarder("int& l = a", l);

    int&& r = 6;
        the_forwarder("int&& r = 6", r);

    the_forwarder("pure rvalue 7", 7);

    the_forwarder("return_prvalue()", return_prvalue());
}

/*
    std::forward<>() has TWO very important applications.
    1. To recycle rvalue (or xvalue)
    2. To forward parameter from one function to another.
*/

int&& sum(int&& s, const int& t)
{
    s += t; 
    return std::forward<int>(s);
}

template<typename OperationType, typename S, typename T>
decltype(auto) forward_it(OperationType operation, S&& s, T&& t)
{
    return operation(std::forward<S>(s), std::forward<T>(t));
}

void test_forward_it_please()
{
    int a = 5;
    int r1 = forward_it(sum, 10, a);

    stream << "r1 = " << r1 << nl;

    int r2 = forward_it([](int&& s, const int& t)
    {
        s+= t; return std::forward<int>(s);
    }, 20, a);

    stream << "r2 = " << r2 << nl;
}

int main()
{
    // test_parameter_distortion();
    // test_std_forward();
    // test_remove_reference();
   // test_non_deduced_context();
    // test_forward();
   //  the_caller();
    test_forward_it_please();
}
