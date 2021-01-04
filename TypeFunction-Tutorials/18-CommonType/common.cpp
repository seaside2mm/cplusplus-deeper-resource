#include "pch.h"
#include "../include/tft_type_lib.h"

auto stream = tpf::wstream();
auto nl = stream.get_console_out();

/*
    tpf::stream() returns stream class based on char - type
    tpf::wstream() returns stream class based on wchar_t - type

    But they are interoperable ... that is... they seem to work identically
    but on modern Operating Systems work using wchar_t internally,
    especially Windows operating system, wstream() class can be more efficient.

    Which one to use... is just a matter of preference
*/

template<typename... Types>
void test_type_names(Types... args)
{
    // tpf::type_list_t is defined in tpf_type_util.h
    //  
    //  template<typename... Types>
    //  struct type_list_t
    //  {  };
    // type_list_t can take zero or more of types
    // 
    // using type_t = tpf::type_list_t<int>;

    // stream << "type_t - " << type_t{} << nl;
    // stream << "tpf::type_list_t<double>{} - " << tpf::type_list_t<double>{} << nl;

    // // display the type of the argument 1
    // stream << "tpf::type_get_list(...) - " << tpf::type_get_list(1) << nl;

    // int a = 5;
    // stream << "tpf::type_get_list(int&, int&&) - " << tpf::type_get_list(a, 5) << nl;

    // stream << "types of args - " << tpf::type_get_list(args...) << nl;
    // stream << "types of std::forward<Types>(args) - " << tpf::type_get_list( std::forward<Types>(args)... ) << nl;

    using typelist_t = tpf::type_list_t<int, short, double>;

    auto types = tpf::type_list_v<int, short, double>;

    stream << "typelist_t - " << typelist_t{} << nl;
    stream << "types - " << types << nl;
}

//////////////////////////
template<typename... Types>
void test_type_names_references(Types&&... args)
{
    // tpf::type_list_t is defined in tpf_type_util.h
    //  
    //  template<typename... Types>
    //  struct type_list_t
    //  {  };
    // type_list_t can take zero or more of types
    // 
    // using type_t = tpf::type_list_t<int>;

    // stream << "type_t - " << type_t{} << nl;
    // stream << "tpf::type_list_t<double>{} - " << tpf::type_list_t<double>{} << nl;

    // // display the type of the argument 1
    // stream << "tpf::type_get_list(...) - " << tpf::type_get_list(1) << nl;

    // int a = 5;
    // stream << "tpf::type_get_list(int&, int&&) - " << tpf::type_get_list(a, 5) << nl;

    stream << "types of args - " << tpf::type_get_list(args...) << nl;
    stream << "types of std::forward<Types>(args) - " << tpf::type_get_list( std::forward<Types>(args)... ) << nl;

    // using typelist_t = tpf::type_list_t<int, short, double>;

    // auto types = tpf::type_list_v<int, short, double>;

    // stream << "typelist_t - " << typelist_t{} << nl;
    // stream << "types - " << types << nl;
}

void test_common_type_of_s_and_t()
{
    namespace hidden = tft::hide;

    // using common1_t = hidden::common_type_of_s_and_t<int, int>;
    // stream << "common1_t - " << tpf::type_list_v<common1_t> << nl;

    // using common2_t = hidden::common_type_of_s_and_t<int, double>;
    // stream << "common2_t - " << tpf::type_list_v<common2_t> << nl;

    // using common3_t = hidden::common_type_of_s_and_t<int, int*>;
    // stream << "common3_t - " << tpf::type_list_v<common3_t> << nl;

    // One of the reasons that we cannot use std::common_type_t<> is that
    // it fails to build when the type function fails.
    
    // it won't compile at all... so we cannot use std::commont_type_t
    // this is one of the primary reasons we cannot use std::common_type_t in 
    // our development
    // using common4_ t = std::common_type_t<int, int*>;

    stream.std() << std::boolalpha;
    stream << "int, double - common type exists ? " 
        << tft::exist_common_type_v<int, double> << ", common type - " 
            << tpf::type_list_v<hidden::common_type_of_s_and_t<int, double>> << nl;

    stream << "int, double - operable ? " << tft::are_types_operable_v<int, double> 
        << ", common type - " << tpf::type_list_v<hidden::common_type_of_s_and_t<int, double>> << nl;
 }

 void test_common_type_t_1()
 {
     using common1_t = tft::common_type_t<int>;
     stream << "common1_t = " << tpf::type_list_v< common1_t > << nl;
     
     using common2_t = tft::common_type_t<int, double>;
     stream << "common2_t = " << tpf::type_list_v< common2_t > << nl;

     using common3_t = tft::common_type_t<int, int*>;
     stream << "common3_t = " << tpf::type_list_v< common3_t > << nl;
 }

 void test_common_type_t_2()
 {
     stream.std() << std::boolalpha;

     using types1_t = tpf::type_list_t<int>;
     using common1_t = tft::common_type_t<types1_t>;
     stream << types1_t{} << "\'s common type is " << tpf::type_list_v<common1_t> << nl; 

     using types2_t = tpf::type_list_t<int, double>;
     using common2_t = tft::common_type_t<types2_t>;
     stream << types2_t{} << "\'s common type is " << tpf::type_list_v<common2_t> << nl; 

     using types3_t = tpf::type_list_t<int>;
     stream << types3_t{} << "\'s common type is " << tft::common_typelist_v<types3_t> 
         << ", does have common type? " << tft::common_type_v<types3_t> << nl;

     using types4_t = tpf::type_list_t<int, double>;
     stream << types4_t{} << "\'s common type is " << tft::common_typelist_v<types4_t> 
        << ", does have common type? " << tft::common_type_v<types4_t> << nl;

    using types5_t = tpf::type_list_t<int, char*>;
    stream << types5_t{} << "\'s common type is " 
        << tft::common_typelist_v<types5_t> 
        << ", does have common type? " << tft::common_type_v<types5_t> << nl;

    using types6_t = tpf::type_list_t<char, unsigned char, short, unsigned short, int, unsigned, long, unsigned long>;
    stream << types6_t{} << "\'s common type is " 
        << tft::common_typelist_v<types6_t> 
        << ", does have common type? " << tft::common_type_v<types6_t> << nl;

    using types7_t = tpf::type_list_t<double, unsigned char, short, unsigned short, int, unsigned, long, unsigned long>;
    stream << types7_t{} << "\'s common type is " 
        << tft::common_typelist_v<types7_t> 
        << ", does have common type? " << tft::common_type_v<types7_t> << nl;


    using types8_t = tpf::type_list_t<double*, unsigned char, short, unsigned short, int, unsigned, long, unsigned long>;
    stream << types8_t{} << "\'s common type is " 
        << tft::common_typelist_v<types8_t> 
        << ", does have common type? " << tft::common_type_v<types8_t> << nl;
 }

 // this function does not work
 // don't use this function
template<typename T>
using make_signed_t = std::conditional_t< tft::is_integer_v<T>, // if true
    std::make_signed_t<T>, // make T signed integer
        T // otherwise return T
        >;
        
 void test_is_integer()
 {
     stream.std() << std::boolalpha;

     stream << "int - " << tft::is_integer_v<int> << nl;
     stream << "double - " << tft::is_integer_v<double> << nl;
     stream << "bool - " << tft::is_integer_v<bool> << nl;
     stream << "int* - " << tft::is_integer_v<int*> << nl;

     using r1_t = unsigned int;
     using s1_t = make_signed_t<r1_t>;
     stream << "type of s1_t = " << tpf::type_list_v<r1_t> 
        << " - " << tpf::type_list_v<s1_t> << nl;

    //  using r2_t = double;
    //  using s2_t = make_signed_t<r2_t>;
    //  stream << "type of s3_t = " << tpf::type_list_v<r2_t> 
    //     << " - " << tpf::type_list_v<s2_t> << nl;

    // template<typename double>
    // using make_signed_t = std::conditional_t< false
    //  std::make_signed_t<double>, // this part does not compile
    //  double // otherwise return T
    //  >;

 }

void test_is_signed_common_type_t()
{
    using u1_t = unsigned int;
    using s1_t = tft::signed_common_type_t<u1_t>;
    stream << "u1_t - " << tpf::type_list_v<u1_t> << ", "<< tpf::type_list_v<s1_t> << nl;

    using u2_t = tpf::type_list_t<unsigned int, unsigned short>;
    using s2_t = tft::signed_common_type_t<u2_t>;
    stream << "u2_t - " << u2_t{} << ", "<< tpf::type_list_v<s2_t> << nl;

    using u3_t = tpf::type_list_t<unsigned int, unsigned short, double>;
    using s3_t = tft::signed_common_type_t<u3_t>;
    stream << "u3_t - " << u3_t{} << ", "<< tpf::type_list_v<s3_t> << nl;

    using u4_t = tpf::type_list_t<unsigned int, unsigned short, double, char*>;
    using s4_t = tft::signed_common_type_t<u4_t>;
    stream << "u4_t - " << u4_t{} << ", "<< tpf::type_list_v<s4_t> << nl;

    using s5_t = tft::signed_common_type_t<unsigned int, unsigned short, double>;
    stream << "u5_t - " << tpf::type_list_t<unsigned int, unsigned short, double>{} 
        << ", "<< tpf::type_list_v<s5_t> << nl;

    using s6_t = tft::signed_common_type_t<int*, unsigned int, unsigned short, double>;
    stream << "u6_t - " << tpf::type_list_t<int*, unsigned int, unsigned short, double>{} 
        << ", "<< tpf::type_list_v<s6_t> << nl;
}

// template<typename T, typename... Types>
// auto create_container_simple(T&& first, Types&&... args)

template<typename T, typename... Types>
auto create_container_simple(T first, Types... args)
{
    // STEP 1. Collect all types into type_list_t
    using types_t = tpf::type_list_t<T, Types...>;

    // tft::common_type_v<types_t> test if common type exists
    if constexpr( tft::common_type_v<types_t> )
    {
        // STEP 2. Extract common type from T, Types...
        using common_t = tft::signed_common_type_t<types_t>;

        // if all arguments first... args ... can be
        // contained in a vector, then we create and return 
        // a vector that contains all elements
        return std::vector<common_t>{ static_cast<common_t>(first),
            static_cast<common_t>(args)... };
    }
    else
    {
        // STEP 3. We cannnot create a vector container
        // because there are elements that cannot be contained
        // to a single vector class

        // so, we create a tuple and return it to the caller
        return std::tuple<T, Types...>{ first, args...};
    }
 }

 void test_create_simple_container()
 {
     // all elements can be contained in a vector<double>
     auto c1 = create_container_simple(1, 2.5f, 3.14, 3l, 4ul, 6ull, 7ll);
     stream << "c1 = " << c1 
            << "\n container type: " << tpf::type_get_list(c1) << nl;

    // all elements can be containerd in a vector<long long>
    auto c2 = create_container_simple(1, 4u, 3l, 4ul, 6ull, 7ll);
    stream << "c2 = " << c2 
            << "\n container type: " << tpf::type_get_list(c2) << nl;

    // these elements cannot be contained in a vector
    // so the create_container_simple() create a tuple
    auto c3 = create_container_simple(1, 4u, "James Dean", 4ul, L"Sophie Turner", 7ll);
    stream << "c3 = " << c3
            << "\n container type: " << tpf::type_get_list(c3) << nl;
 }

 template<typename T, typename... Types>
 void type_function_demo(T first, Types... args)
 {
    // collect all types
    using types_t = tpf::type_list_t<T, Types...>;
     
    stream << "T, Types... - " << types_t{} << nl;

    stream << "first, args... - " << tpf::type_get_list(first, args...) << nl;

    stream << "int - " << tpf::type_list_t<int>{} << nl;
    stream << "int - " << tpf::type_list_v<int> << nl;

    stream << "int, short, long, double - " << tpf::type_list_t<int, short, long, double>{} << nl;
    stream << "int, short, long, double - " << tpf::type_list_v<int, short, long, double> << nl;

    stream << "1, 2, 3u, \'c\', \"string\", L\"Wide String\" - " << 
        tpf::type_get_list(1, 2, 3u, 'c', "string", L"Wide String") << nl;
 }

 void test_type_function_demo()
 {
     type_function_demo(1, 2, 3u, 'c', "string", L"Wide String");

 }

int main()
{
    // test_type_names();
    // int a = 5;
    // test_type_names(a, 1, 2.0, 5u, 6ll, 7ull);

    // int a = 5;
    // test_type_names_references(a, 1, 2.0, 5u, 6ll, 7ull);
    // test_common_type_of_s_and_t();

    // test_common_type_t_1();
    // test_common_type_t_2();
    // test_is_integer();

    // test_is_signed_common_type_t();
    // test_create_simple_container();
    test_type_function_demo();
}