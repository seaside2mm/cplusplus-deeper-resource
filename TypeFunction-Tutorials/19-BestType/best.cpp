#include "pch.h"
/*
    C++11 introduced std::tuple
    C++17 introduced std::variant and std::any

    These type functions that we are going to implement are
    used to efficiently and easily maninpulate these new container classes
*/

auto stream = tpf::stream();
auto nl = stream.get_console_out();

struct tag
{
    static constexpr size_t idx_age = 0;
    using age_t = int;

    static constexpr size_t idx_weight = 1;
    using weight_t = double;

    static constexpr size_t idx_name = 2;
    using name_t = std::string;
};

void motivation()
{
    auto stream = tpf::stream();
    auto nl = stream.get_console_out();

    // using one_of_t = std::variant<int, double, std::string>
    using one_of_t = std::variant<tag::age_t, tag::weight_t, tag::name_t>;

    using bag_t = std::vector<one_of_t>;

    bag_t mybag { 20, 60.6, "Sophie Turner", "Game of Thrones" };

    auto process_mybag = [&stream, &nl](auto& item, auto index)
    {
        switch(index)
        {
            case tag::idx_age:
                stream << "Age is "<< item << nl; break;

            case tag::idx_weight:
                stream << "Weight is "<< item << nl; break;

            case tag::idx_name:
                stream << "Name is "<<item << nl; break;
       }

    }; // do not forget this ; here

    stream << "my bag containes \n"
        << mybag << nl << nl;
        
    for(auto& var: mybag)
        tpf::process_variant(var, process_mybag);
}

/*
    Suppose we are given a set of types {int, double, std::string},
    and we are also given a type int, we want to know if int is in the set.
    We call this function find_same_type_st
*/

struct NoTypeDummy{};

// the set is {Type, Types...}
// ArgType is the type to test if it is in the set
template<typename ArgType, typename Type, typename... Types>
struct find_same_type_st
{
    static constexpr bool value = find_same_type_st<ArgType, Type>::value ? true :
        find_same_type_st<ArgType, Types...>::value;

    using type = std::conditional_t< find_same_type_st<ArgType, Type>::value,
        Type, typename find_same_type_st<ArgType, Types...>::type>;
};

template<typename ArgType, typename Type>
struct find_same_type_st<ArgType, Type>
{
    static constexpr bool value = std::is_same_v<Type, ArgType>;
    using type = std::conditional_t<value, Type, NoTypeDummy>;
};

template<typename ArgType, typename Type, typename... Types>
constexpr auto same_type_v = find_same_type_st<ArgType, Type, Types...>::value;

template<typename ArgType, typename Type, typename... Types>
using same_type_t = typename find_same_type_st<ArgType, Type, Types...>::type;

void test_find_same_type()
{
    stream.std() << std::boolalpha;

    /*
    Suppose we are given a set of types {int, double, std::string},
    and we are also given a type int, we want to know if int is in the set.
    We call this function find_same_type_st
    */

   stream << "{int, double, std::string} containes int : "
        << same_type_v<int, int, double, std::string> << nl;

   stream << "{int, double, std::string} containes short : "
    << same_type_v<short, int, double, std::string> << nl;

    stream << tpf::type_list_v< same_type_t<int, int, double, std::string> > << nl;

    stream << tpf::type_list_v< same_type_t<short, int, double, std::string> > << nl;
 }

// the set is {Type, Types...}
// ArgType is the type to test if it can be used by a type in the set
// to construct an instance of the type
// for example, if we are given {int, double, std::string},
// and another type const char*, we want to find a type such that 
// 
// int i { "literal string" }, std::is_constructible_v<int, const char*> - fails
// double d { "literal string"}, std::is_constructible_v<double, const char*> - fails
// std::string s { "literal string"}, std::is_constructible_v<std::string, const char*> - succeeds
//
// std::is_constructible_v<std::string, const char*> -- return true
// 
template<typename ArgType, typename Type, typename... Types>
struct find_constructible_type_st
{
    static constexpr bool value = find_constructible_type_st<ArgType, Type>::value ? true :
        find_constructible_type_st<ArgType, Types...>::value;

    using type = std::conditional_t< find_constructible_type_st<ArgType, Type>::value,
        Type, typename find_constructible_type_st<ArgType, Types...>::type>;
};

template<typename ArgType, typename Type>
struct find_constructible_type_st<ArgType, Type>
{
    static constexpr bool value = std::is_constructible_v<Type, ArgType>;
    using type = std::conditional_t<value, Type, NoTypeDummy>;
};

template<typename ArgType, typename Type, typename... Types>
constexpr auto constructible_type_v = find_constructible_type_st<ArgType, Type, Types...>::value;

template<typename ArgType, typename Type, typename... Types>
using constructible_type_t = typename find_constructible_type_st<ArgType, Type, Types...>::type;


void test_find_constructible_type()
{
    stream.std() << std::boolalpha;

    /*
    Suppose we are given a set of types {int, double, std::string},
    and we are also given a type const char*, we want to know if there exists
    a type in the set, that can be constructed using const char*.
    We call this function find_constructible_type_st
    */

    stream << "one of {int, double, short} is contructible using const char* : "
        << constructible_type_v<const char*, int, double, short> << nl;

    stream << "one of {int, double, std::string} is contructible using const char* : "
        << constructible_type_v<const char*, int, double, std::string> << nl;

    stream << tpf::type_list_v< constructible_type_t<const char*, int, double, short> > << nl;

    stream << tpf::type_list_v< constructible_type_t<const char*, int, double, std::string> > << nl;
 }

template<typename ArgType, typename Type, typename... Types>
struct find_best_type_st
{
    static constexpr bool value = same_type_v<ArgType, Type, Types...> ? true :
        constructible_type_v<ArgType, Type, Types...>;

    using type = std::conditional_t< same_type_v<ArgType, Type, Types...>,
        same_type_t<ArgType, Type, Types...>,
        constructible_type_t<ArgType, Type, Types...>>;
};

template<typename ArgType, typename Type, typename... Types>
constexpr auto best_type_v = find_best_type_st<ArgType, Type, Types...>::value;

template<typename ArgType, typename Type, typename... Types>
using best_type_t = typename find_best_type_st<ArgType, Type, Types...>::type;

void test_find_best_type()
{
    stream.std() << std::boolalpha;

    stream << "the best match of short out of {int, double, short} exists : "
        << best_type_v<short, int, double, short> << " - " <<
            tpf::type_list_v< best_type_t<short, int, double, short> > << nl;

    stream << "the best match of const char* out of {int, double, std::string} exists : "
        << best_type_v<const char*, int, double, std::string> << " - " <<
            tpf::type_list_v< best_type_t<const char*, int, double, std::string> > << nl;

    stream << "the best match of const char* out of {int, double, short} exists : "
        << best_type_v<const char*, int, double, short> << " - " <<
            tpf::type_list_v< best_type_t<const char*, int, double, short> > << nl;
 }

int main()
{
    // motivation();
    // test_find_same_type();
    // test_find_constructible_type();
    test_find_best_type();
}