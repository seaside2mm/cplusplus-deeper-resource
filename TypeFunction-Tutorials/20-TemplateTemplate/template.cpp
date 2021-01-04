#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void test_find_same_type()
{
    stream.std() << std::boolalpha;

    /*
    Suppose we are given a set of types {int, double, std::string},
    and we are also given a type int, we want to know if int is in the set.
    We call this function find_same_type_st
    */

   stream << "{int, double, std::string} containes int : "
        << tpf::same_type_v<int, int, double, std::string> << nl;

   stream << "{int, double, std::string} containes short : "
    << tpf::same_type_v<short, int, double, std::string> << nl;

    stream << tpf::type_list_v< tpf::same_type_t<int, int, double, std::string> > << nl;

    stream << tpf::type_list_v< tpf::same_type_t<short, int, double, std::string> > << nl;
 }



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
        << tpf::constructible_type_v<const char*, int, double, short> << nl;

    stream << "one of {int, double, std::string} is contructible using const char* : "
        << tpf::constructible_type_v<const char*, int, double, std::string> << nl;

    stream << tpf::type_list_v< tpf::constructible_type_t<const char*, int, double, short> > << nl;

    stream << tpf::type_list_v< tpf::constructible_type_t<const char*, int, double, std::string> > << nl;
 }

void test_find_best_type()
{
    stream.std() << std::boolalpha;

    stream << "the best match of short out of {int, double, short} exists : "
        << tpf::best_type_v<short, int, double, short> << " - " <<
            tpf::type_list_v< tpf::best_type_t<short, int, double, short> > << nl;

    stream << "the best match of const char* out of {int, double, std::string} exists : "
        << tpf::best_type_v<const char*, int, double, std::string> << " - " <<
            tpf::type_list_v< tpf::best_type_t<const char*, int, double, std::string> > << nl;

    stream << "the best match of const char* out of {int, double, short} exists : "
        << tpf::best_type_v<const char*, int, double, short> << " - " <<
            tpf::type_list_v< tpf::best_type_t<const char*, int, double, short> > << nl;
 }

void test_find_best_type_list()
{
    stream.std() << std::boolalpha;

    using typelist1_t = tpf::type_list_t<int, double, short>;

    stream << "the best match of short out of " << typelist1_t{} << " exists : "
        << tpf::best_type_v<short, typelist1_t> << " - " << tpf::type_list_v< tpf::best_type_t<short, typelist1_t> > << nl;


    using typelist2_t = tpf::type_list_t<int, double, std::string>;

    stream << "the best match of const char* out of " << typelist2_t{} << " exists : "
        << tpf::best_type_v<const char*, typelist2_t> << " - " << tpf::type_list_v< tpf::best_type_t<const char*, typelist2_t> > << nl;

    using typelist3_t = tpf::type_list_t<int, double, short>;
    
    stream << "the best match of const char* out of " << typelist3_t{} << " exists : "
        << tpf::best_type_v<const char*, typelist3_t> << " - " << tpf::type_list_v< tpf::best_type_t<const char*, typelist3_t> > << nl;
}

void test_tpf_any()
{
    /*
        C++17 introduced std::any class. And I created tpf::any based on std::andy.
        But tpf::any has a bug and I will fix it using best_type_t and best_type_v.
    */

//    using any_t = tpf::any<int, double, const char*>;

//    any_t a{ "literal string" };

//    stream << "a = " << a << nl;

    using any_t = tpf::any<int, double, std::string>;

   any_t a{ "literal string" };

   stream << "a = " << a << nl;

}

int main()
{
    // test_find_same_type();
    // test_find_constructible_type();
    // test_find_best_type();
    //  test_find_best_type_list();
    test_tpf_any();
}