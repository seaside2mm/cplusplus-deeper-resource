#include "pch.h"
#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

/*
    In our next session, we will learn "is-a" relationship and extend C++ Standard Library
    to meet our needs.
*/

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

template<>
struct container_element_st<const char*> // non reference
{
    using type = char;
    using element_type = std::string;
    static constexpr bool is_character_array = true;
    static constexpr size_t array_count = 0;
};

template<size_t N>
struct container_element_st<wchar_t[N]> // non reference
{
    using type = wchar_t;
    using element_type = std::wstring;
    static constexpr bool is_character_array = true;
    static constexpr size_t array_count = N;
};

template<>
struct container_element_st<const wchar_t*> // non reference
{
    using type = wchar_t;
    using element_type = std::wstring;
    static constexpr bool is_character_array = true;
    static constexpr size_t array_count = 0;
};

template<typename T, size_t N>
struct container_element_st<T[N]> // array but not character array
{
    using type = T;
    using element_type = std::vector<T>;
    static constexpr bool is_character_array = false;
};

template<typename T, size_t N>
struct container_element_st<T(&)[N]>
{
    using type = T;
    using element_type = std::vector<T>;
    static constexpr bool is_character_array = false;
};

// from initializer_list to vector conversion
template<typename T>
struct container_element_st<std::initializer_list<T>>
{
    using type = T;
    using element_type = std::vector<T>;
    static constexpr bool is_character_array = false;
};

template<typename T>
constexpr auto is_character_array_v = container_element_st<tft::remove_const_reference_t<T>>::is_character_array;

template<typename T>
constexpr auto array_count_v = container_element_st<tft::remove_const_reference_t<T>>::array_count;

template<typename T>
using element_flat_t = typename container_element_st<tft::remove_const_reference_t<T>>::type;

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

///////////////////////////////////////////////////////////////
// we have to convert const char* to std::string
// const wchar_t* to std::wstring
// convert array, int[] to std::vector<int>
// std::initializer_list<> to std::vector<>

template<typename Type>
auto element_to_container(Type&& arg)
{
    // array... such char[N], or int[N], etc.
    if constexpr(std::is_array_v<tft::remove_const_reference_t<Type>>)
    {
        if constexpr(is_character_array_v<Type>)
        { 
                // character array, we convert 
                // const char* to std::string
                // const wchar_t* to std::wstring
                // char[N] to std::string
                // wchar_t[N] to std::wstring
                using element_t = container_element_t<Type>;
                size_t size = array_count_v<Type>;
                const auto& last_character = arg[size-1];
                
                // element_flat_t<Type> returns eiter char or wchar_t
                // testing if zero terminated character
                if(element_flat_t<Type>{} == last_character)
                    return element_t(decay_array(arg)); // decay_array() converts from char[] tp char*
                else // not zero termniating character array
                    return element_t{ std::cbegin(arg), std::cend(arg)};
        }
        else
        {   
            // non-character array
            using element_t = container_element_t<Type>;

            return element_t{std::cbegin(arg), std::cend(arg)};
        }

    }
    else
    {
        // non array case
        using element_t = container_element_t<Type>;
        return element_t{ std::forward<Type>(arg) };
    }
}

// this belongs to non-deduced context
template<typename Type>
auto element_to_container(std::initializer_list<Type>& lst)
{
    // using element_t = container_element_t<std::initializer_list<Type>>;
    using element_t = container_element_t<decltype(lst)>;

    // conversion from std::initializer_list<Type> to std::vector<Type>
    return element_t{ lst };
}

// this belongs to non-deduced context
template<typename Type>
auto element_to_container(std::initializer_list<Type>&& lst)
{
    // using element_t = container_element_t<std::initializer_list<Type>>;
    using element_t = container_element_t<decltype(lst)>;

    // conversion from std::initializer_list<Type> to std::vector<Type>
    return element_t{ std::forward<std::initializer_list<Type>>(lst) };
}

template<typename Type, typename... Types>
auto make_tuple(Type&& first, Types&&... args)
{
    using container_t = std::tuple<container_element_t<Type>, container_element_t<Types>...>;

    return container_t{ element_to_container(std::forward<Type>(first)),
        element_to_container(std::forward<Types>(args))... };
}

template<auto Index, typename CntrType>
decltype(auto) get(CntrType& cntr)
{
    return std::get<Index>(cntr);
}

template<auto Index, auto... Indices, typename CntrType>
decltype(auto) get(CntrType& cntr)
{
    if constexpr(sizeof...(Indices) > 0)
        return get<Indices...>(get<Index>(cntr));
    else
        return get<Index>(cntr);
}

template<typename IndexType, auto Index, auto... Indices, typename CntrType>
decltype(auto) get(CntrType& cntr, IndexType index)
{
    if constexpr(sizeof...(Indices) >0 )
        return get<Indices...>(get<Index>(cntr))[index];
    else
        return get<Index>(cntr)[index];
} 

void test_element_to_container()
{
    using literal_string_t = const char*;

    auto str1 = element_to_container("This is literal string");
    stream << "str1 = " << str1<< ",\n type: "
        << Tpf_GetTypeCategory(str1) << nl;

    int array[] {1, 2, 3, 4, 5};

    auto vtr1 = element_to_container(array);

    stream << "vtr1 = " << vtr1<< ",\n type: "
        << Tpf_GetTypeCategory(vtr1) << nl;


    auto vtr2 = element_to_container( { 3.0, 5.0, 6.0 } );

    stream << "vtr2 = " << vtr2<< ",\n type: "
        << Tpf_GetTypeCategory(vtr2) << nl;
  
}

void test_make_tuple()
{
    const char* msg1 = "Literal String";
    const wchar_t* msg2 = L"Wide character string";

    auto t1 = make_tuple(msg1, "Thomas Kim", L"James Dean", 20, "Sophie Turner", msg2);

    stream << "t1 = " << t1 << nl;

    using t1_0_th_t = std::tuple_element_t<0, decltype(t1)>;
    stream << "t1-0-th element: " << std::get<0>(t1) << ",\n"
        << Tpf_GetTypeName(t1_0_th_t) << nl;

    using t1_5_th_t = std::tuple_element_t<5, decltype(t1)>;
    stream << "t1-5-th element: " << std::get<5>(t1) << ",\n"
        << Tpf_GetTypeName(t1_5_th_t) << nl;
} 

void test_advanced_make_tuple()
{
    auto tpl = make_tuple(
        make_tuple(1, 3.14, "James"),
        make_tuple( make_vector(1, 2, 3, 4, 5), make_vector(3.0, 5.0, 7.0), 
        make_tuple("Steven", L"Sophie", 40.0f)),
        make_vector("James", "Seveten"));

    stream << tpl << nl;

    stream << get<0>(tpl) << nl;
    stream << get<1>(tpl) << nl;
    stream << get<1, 0>(tpl) << nl;
    stream << get<1, 1>(tpl) << nl;
    stream << get<1, 2>(tpl) << nl;

     stream << get<1, 2, 0>(tpl) << nl;
     stream << get<1, 2, 1>(tpl) << nl;
     stream << get<1, 2, 2>(tpl) << nl;

     stream << get<1>(tpl) << nl;
     stream << get<1, 0>(tpl) << nl;

    stream << nl;
    for(auto& e: get<1, 0>(tpl))
        stream << e << nl;

    stream << nl;
    
}

// this is primary template class
// the primary class template determines the count of template arguments
// since this primary class template has
// 3 types... bool, TrueType, FalseType
// we have to privode 3 template arguments in the specialization
template<bool bWhich, typename TrueType, typename FalseType>
struct conditional_st;

// this is a specialization, when bWhich is true
template<typename TrueType, typename FalseType>
struct conditional_st<true, TrueType, FalseType>
{
    using type = TrueType; // when bWhich is true, we return TrueType
};

// this is a specialization, when bWhich is false
template<typename TrueType, typename FalseType>
struct conditional_st<false, TrueType, FalseType>
{
    using type = FalseType; // when bWhich is false, we return FalseType
};

template<bool bWhich, typename TrueType, typename FalseType>
using conditional_t = typename conditional_st<bWhich, TrueType, FalseType>::type;

/*
    Whenever we create a new type function, ensure that it works as expected.
    Test the new type function thoroughly, and find out its limitations as well.
*/

void test_conditional_t()
{
    using select_int_t = conditional_t<true, int, double>;
    using select_double_t = conditional_t<false, int, double>;

    stream << "select_int_t: " << Tpf_GetTypeName(select_int_t) << nl;
    stream << "select_double_t: " << Tpf_GetTypeName(select_double_t) << nl;
} 


// this is primary class template
// primary class template determines the template arguments count
// this primary class templates takes 1 or more arguments
template<typename T, typename... Types>
struct are_all_the_same_type_st;

template<typename T1, typename T2>
struct are_all_the_same_type_st<T1, T2> // 2 template arguments... conformant to the primary class template
{                               // we regard T1 and T2 are different
    static constexpr bool value = false; // T1 and T2 are regarded as different type
};

template<typename T>
struct are_all_the_same_type_st<T, T> // 2 template arguments are the same... T and T
{
    static constexpr bool value = true;
};

template<typename T>
struct are_all_the_same_type_st<T>
{
    static constexpr bool value = true; // why true?... you will soon see why
                                        // if && operation is to hold, this single argument case should be true
                                        // this is unit 1
};

template<typename T1, typename T2, typename... Types>
struct are_all_the_same_type_st<T1, T2, Types...>
{
    static constexpr bool value = are_all_the_same_type_st<T1, T2>::value 
                                  &&  // because we are testing if all types T1, T2, ... Types are the same
                                      // we can regard && operation as multiplication
                                  are_all_the_same_type_st<T2, Types...>::value; // recursion
};

template<typename T, typename... Types>
constexpr auto are_all_the_same_type_v = are_all_the_same_type_st<T, Types...>::value;

template<typename T, typename... Types>
constexpr auto are_all_the_same_flat_type_v = are_all_the_same_type_v<tft::remove_const_reference_t<T>,
                                    tft::remove_const_reference_t<Types>...>;

/*
    Whenever we create a new type function, we have to thoroughly test
    also have to find out its limitations as well
*/

void test_are_all_the_same_type_v()
{
    stream.std() << std::boolalpha;

    stream << "int: " << are_all_the_same_type_v<int> << nl;
    stream << "int, int: " << are_all_the_same_type_v<int, int> << nl;
    stream << "int, int, int: " << are_all_the_same_type_v<int, int, int> << nl;

    stream << "int, double: " << are_all_the_same_type_v<int, double> << nl;
    stream << "int, int, double: " << are_all_the_same_type_v<int, int, double> << nl;

    stream << "int, int&: " << are_all_the_same_type_v<int, int&> << nl;

    stream << "int, const int: " << are_all_the_same_type_v<int, const int> << nl;

    stream << "int, const int&: " << are_all_the_same_type_v<int, const int&> << nl;
}

void test_are_all_the_same_flat_type_v()
{
    stream.std() << std::boolalpha;

    stream << "int: " << are_all_the_same_flat_type_v<int> << nl;
    stream << "int, int: " << are_all_the_same_flat_type_v<int, int> << nl;
    stream << "int, int, int: " << are_all_the_same_flat_type_v<int, int, int> << nl;

    stream << "int, double: " << are_all_the_same_flat_type_v<int, double> << nl;
    stream << "int, int, double: " << are_all_the_same_flat_type_v<int, int, double> << nl;

    stream << "int, int&: " << are_all_the_same_flat_type_v<int, int&> << nl;

    stream << "int, int&&: " << are_all_the_same_flat_type_v<int, int&&> << nl;
    
    stream << "int&, int&&: " << are_all_the_same_flat_type_v<int&, int&&> << nl;

    stream << "int, const int: " << are_all_the_same_flat_type_v<int, const int> << nl;

    stream << "int, const int&: " << are_all_the_same_flat_type_v<int, const int&> << nl;
}

template<typename T, typename... Types>
using select_container_member_type_t = 
    conditional_t< are_all_the_same_flat_type_v<T, Types...>, // if all types are the same
        std::vector<container_element_t<T>>, // return std::vector<> class
        std::tuple< container_element_t<T>, container_element_t<Types>... > >;

/*
    Whenever we create a new type function, we always have to test its validity.
*/

void test_select_container_member_type_t()
{
    using container_vector_t = select_container_member_type_t<int, int, int>;

    stream << "container_vector_t: " << Tpf_GetTypeName(container_vector_t) << nl;

    using container_tuple_t = select_container_member_type_t<const char*, int, double>;

    stream << "container_tuple_t: " << Tpf_GetTypeName(container_tuple_t) << nl;

    using container_tuple_vector_t = select_container_member_type_t<const char*, int(&)[5], double>;

    stream << "container_tuple_vector_t: " << Tpf_GetTypeName(container_tuple_vector_t) << nl;


}

// this container can contain any type... any format... in arbitrary tree format
template<typename MemberType>
class container
{
    private:
        MemberType m_value;

    public:

        // this is constructor untied from container<MemberType>
        // or is called template member constructor
        template<typename T>
        container(T arg=T{}): m_value{arg}
        {
            
        }

        // this is also untied from container<MemberType>
        template<typename T1, typename T2, typename... Types>
        container(T1 arg1, T2 arg2, Types... args): 
            m_value{ element_to_container(arg1), 
            element_to_container(arg2), element_to_container(args)... }
        {
            
        }

        template<typename CharType>
        friend decltype(auto) operator<<(tpf::stream_t<CharType>& os, const container& cntr)
        {
            os << cntr.m_value; return os;
        }
};

// we will make class template argument (or parameter) deduction guide here

 template<typename T> 
    container(T) -> container<T>;

template<typename T1, typename T2, typename... Types, 
    typename CntrType = select_container_member_type_t<T1, T2, Types...>>
    container(T1, T2, Types...) -> container<CntrType>;
            
void test_container()
{
    container c(5);

    stream << "c = " << c << nl << nl;

    container v(1, 2, 3, 4, 5, container("Sophia", "James Dean"), container(L"Game of Throne", L"Thomas Kim"));

    stream << "v = " << v << nl << nl;

    container t(1, 2.5, "Test", L"Wide Literal", container(3.0, 5.0, 6.0) );

    stream << "t = " << t << nl << nl;
    
}

int main()
{
    // test_element_to_container();
    // test_make_tuple();
    // test_advanced_make_tuple();
     // test_conditional_t();
     // test_are_all_the_same_type_v();
     // test_are_all_the_same_flat_type_v();
     // test_select_container_member_type_t();

     test_container();
}