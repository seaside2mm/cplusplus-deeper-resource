#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

template<typename T, size_t N> // T is type template parameter, N is non-type template parameter
void take_static_array( T(&array)[N] ) // T(&array)[N] is a lvalue reference to a array of type T with N elements
{
    stream << "Type " << Tpf_GetTypeName(T) 
        << " array[" << N << "] is passed as parameter" << nl;

    stream << array << nl;
}

template<typename T, auto N1, auto N2> // T is type template parameter, N1 and N2 are non-type template parameter
void take_static_array( T(&array)[N1][N2] ) // two dimensional static array
{
    stream << "Type " << Tpf_GetTypeName(T) 
        << " array[" << N1 << "][" << N2 << "] is passed as parameter" << nl;

    stream << array << nl;
}

void test_static_array()
{
    int array[] {1, 2, 3, 4, 5 };
    int array2[][5] {{1, 2, 3, 4, 5 }, {5, 4, 3, 2, 1 } };

    take_static_array(array);
    take_static_array(array2);
}

// decay array to pointer
template<typename T, size_t N>
auto decay_array(T(&array)[N])
{
    return array;
}

// primary class template
template<typename T>
struct container_element_st
{
    using type = T;
    using element_type = T;
    static constexpr bool is_character_array = false;
    static constexpr size_t array_count = 0;
};

template<size_t N>
struct container_element_st<char[N]> // non reference
{
    using type = char;
    using element_type = std::string;
    static constexpr bool is_character_array = true;
    static constexpr size_t array_count = N;
};

template<size_t N>
struct container_element_st<wchar_t[N]> // non reference
{
    using type = wchar_t;
    using element_type = std::wstring;
    static constexpr bool is_character_array = true;
    static constexpr size_t array_count = N;
};

template<typename T>
constexpr auto is_character_array_v = container_element_st<tft::remove_const_reference_t<T>>::is_character_array;

template<typename T>
constexpr auto array_count_v = container_element_st<tft::remove_const_reference_t<T>>::array_count;

template<typename T>
using element_flat_t = typename container_element_st<tft::remove_const_reference_t<T>>::type;

template<typename T, size_t N>
struct container_element_st<T[N]>
{
    using type = T;
    using element_type = std::vector<T>;
};

template<typename T>
using container_element_t = typename container_element_st<tft::remove_const_reference_t<T>>::element_type;

template<typename Type, typename... Types>
auto make_vector(Type&& first, Types&&... args)
{
    if constexpr(std::is_array_v<tft::remove_const_reference_t<Type>>)
    {
        // this is to allow only character array
        static_assert(is_character_array_v<Type>, "should be character array");
            
        using element_t = container_element_t<Type>;
        using container_t = std::vector<element_t>;

        size_t size = array_count_v<Type>;
        const auto& last_character = first[size-1];

        // last character is NULL character
        if(element_flat_t<Type>{} == last_character)
            return container_t{decay_array(first), decay_array(args)...};
        else
        {
            return container_t{element_t{std::cbegin(first), std::cend(first)},
                element_t{std::cbegin(args), std::cend(args)}...};
        }    
    }
    else
    {
        using element_t = tft::remove_const_reference_t<Type>;

        return std::vector<element_t>{ std::forward<Type>(first), std::forward<Types&&>(args)...};
    }
}

template<template<typename, typename...> class CntrType, typename Type, typename... Types>
auto make_container(Type&& first, Types&&... args)
{
    if constexpr(std::is_array_v<tft::remove_const_reference_t<Type>>)
    {
        // this is to allow only character array
        static_assert(is_character_array_v<Type>, "should be character array");
            
        using element_t = container_element_t<Type>;
        using container_t = CntrType<element_t>;

        size_t size = array_count_v<Type>;
        const auto& last_character = first[size-1];

        // last character is NULL character
        if(element_flat_t<Type>{} == last_character)
            return container_t{decay_array(first), decay_array(args)...};
        else
        {
            return container_t{element_t{std::cbegin(first), std::cend(first)},
                element_t{std::cbegin(args), std::cend(args)}...};
        }    
    }
    else
    {
        using element_t = tft::remove_const_reference_t<Type>;

        return CntrType<element_t>{ std::forward<Type>(first), std::forward<Types&&>(args)...};
    }
}

template<typename T>
void print_type(T&& arg)
{
    stream << "type of T: "    << Tpf_GetTypeName(T) 
           <<", type of arg: " << Tpf_GetTypeCategory(decay_array(arg)) << nl;
}

void simple_tip_for_my_audience()
{
    int a = 1;

    std::vector<int> v1{1, 2, 3, 4, 5};

    auto v2 = make_vector(a, 2, 3, 4, 5, 6, 7, 8, 9, 10);

    stream << "v1 = " << v1 << nl;

    stream << "v2 = "<< v2 << nl;

    auto v3 = make_vector("Thomas Kim", "Sophie Turner", "Alice Kim");

    stream << "v3 = "<< v3 << nl;
    
}

void advanced_tip_for_my_audience()
{
    int a = 1;

    auto v2 = make_container<std::vector>(a, 2, 3, 4, 5, 6, 7, 8, 9, 10);

    stream << "v2 = "<< v2 << nl;

    auto v3 = make_container<std::deque>("Thomas Kim", "Sophie Turner", "Alice Kim");

    stream << "v3 = "<< v3 << nl;
    
}


int main()
{
    // test_static_array();
    //simple_tip_for_my_audience();

    /*
        In C/C++, static array of element count zero is not allowed.
    */

    advanced_tip_for_my_audience();
   

}