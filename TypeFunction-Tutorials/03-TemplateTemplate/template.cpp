#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

template<typename Type>
using remove_const_reference_t = std::remove_const_t<std::remove_reference_t<Type>>;


/*
    In old days, "parameter" and "argument" were interchangeable.
    But now, in Modern C++ Programming, 
    "parameter" and "argument" are different concept
*/

template<typename Type, typename... Types> // template parameters
auto build_vector(Type first, Types... args) // function call parameter
{   
    // Type... args is called variadic template function-call parameter
    // typename... Types is called variadic template parameter

    std::vector<Type> v { first, args... };

    return v;
}

void test_build_vector()
{
    // auto v = build_vector(1, 2, 3, 4, 5); // 1, 2, 3, 4, 5 are called arguments
    auto v = build_vector<int, int, int, int, int> // int, int, int, int, int are called type argument
            (1, 2, 3, 4, 5);

    stream << "v = " << v << nl;
}

/*
    build_container_basic() has one drawback.
    In C++ Programming, non-reference parameter gets type-decayed or the argument type 
    is distorted while passing as argument.
*/
template<   template<typename, typename...> class ContainerType, // template template parameter
typename Type, typename... Types>
auto build_container_basic(Type first, Types... args)
{
    ContainerType<Type> c{first, args... };

    return c;
}

void test_build_container_basic()
{
    auto v = build_container_basic<std::deque> // std::deque is called template argument
        (1, 2, 3, 4, 5);

    stream << v << nl;
}

/*
    The biggest drawback of this approach is that
    the passed argument's type is distorted.
    To address this issue, C++11 Standard introduced
    T&& (forwarding reference).
*/
template<typename T>
void PrintType(const T& a) // this is a typical pre-C++11 Standard programming style
{
    stream << "Type of a: " << Tpf_GetTypeCategory(a) << nl;

}

void test_print_type_function()
{
    int a = 5;
    int& lvalue_ref = a;

    // we are passing lvalue refernce to PrintType()
    PrintType( lvalue_ref );
    
    // it does not work
    PrintType( 5 );

}

/*
    1. In case of forwarding reference,
    the type of T is either lvalue reference or non-reference type

    2. the type of function-call parameter a,
    its type is either lvalue refrence or rvalue reference

    3. type of T and the type of function call parameter a
        can be different

    4. when rvalue reference is passed without forwarding
        the type of T is lvalue reference
        and the type of function-call parameter is lvalue reference

    practically, all you need to remember is that
    both the type of T and the type of function-call parameter can
    be changed depending on parameter passing method.

    We can control the type of the passed parameter either using std::forward or std::move.

    In this session, please remember just one thing
    the type of T is not non-reference type.
    it can be lvalue reference type as well.

*/
template<typename T>
void ForwardingReference(const char* msg, T&& a)
{
    stream << msg << nl;
    stream << "The type of a: " << Tpf_GetTypeCategory(a) << nl;
    stream << "The type of T: " << Tpf_GetTypeName(T) << nl;
}

void test_forwarding_reference()
{
    int a = 5;
    int& lvalue_reference = a;
    int&& rvalue_reference = 10;

    ForwardingReference("with int a - ", a);
    ForwardingReference("with std::forward<int>(a)", std::forward<int>(a));
    ForwardingReference("with lvalue_reference - ", lvalue_reference);
    ForwardingReference("with std::forward<int&>(lvalue_reference) - ", std::forward<int&>(lvalue_reference));
    
    ForwardingReference("with rvalue_reference - ", rvalue_reference);
    ForwardingReference("with std::forward<int>(rvalue_reference) - ", std::forward<int>(rvalue_reference));
    ForwardingReference("with std::forward<int&&>(rvalue_reference) - ", std::forward<int&&>(rvalue_reference));
    ForwardingReference("with pure rvalue 20", 20);
}

template< template<typename, typename...> class ContainerType, // template template parameter
typename Type, typename... Types>
auto build_container_advanced(Type&& first, Types&&... args)
{
    // stream << "type of Type: " << Tpf_GetTypeName(Type) << nl;
    // stream << "type of first: " << Tpf_GetTypeCategory(first) << nl;

    using type_t = remove_const_reference_t<Type>;
    
    // it does not work.
    // when you remove reference and const, you have to remove reference first
    // using type_t =  std::remove_reference_t<std::remove_const_t<Type>>;

   ContainerType< type_t > c{ std::forward<Type>(first),  std::forward<Types>(args)... };

    /*
        In C++, we cannot hold reference type in the Container class

    */
    return c;
}

void test_build_container_advanced()
{
    // it works
    stream << "Calling with (1, 2, 3, 4, 5)" << nl;

    auto v = build_container_advanced<std::deque>(1, 2, 3, 4, 5);

    stream << v << nl;
}

void test_build_container_advanced_failed()
{
    const int a = 1;
    // it does not work

    stream << "Calling with (a, 2, 3, 4, 5)" << nl;

    auto v = build_container_advanced<std::deque>(a, 2, 3, 4, 5);

    stream << v << nl;
}


int main()
{
    // test_build_vector();
    // test_build_container_basic();
    
    // test_print_type_function();
    // test_forwarding_reference();

    test_build_container_advanced();
    test_build_container_advanced_failed();


}