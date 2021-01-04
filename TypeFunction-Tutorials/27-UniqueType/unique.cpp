#include "pch.h"

#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void test_unique_type()
{
    using unique = tft::unique_type_t<char, int, char, int, double>;
    stream << "unique: " << unique{} << nl;

    using typelist = tpf::type_list_t<char, char, int, int, double, double>;

    using unique_types = tft::unique_type_t<typelist>;
    
    stream << "unique_types: " << unique_types{} << nl;

    using list1 = tpf::type_list_t<char, int>;
    using list2 = tpf::type_list_t<double, bool>;

    using prepend_t = tft::prepend_type_t<list1, list2>;
    stream << "prepend_t: " << prepend_t{} << nl;

    using append_t = tft::append_type_t<list1, list2>;
    stream << "append_t: " << append_t{} << nl;
    
    stream.std() << std::boolalpha;

    stream << "is char in the list: " << tft::is_type_in_list_v<char, char, int, double> << nl;
    
    stream << "is char in the list: " << tft::is_type_in_list_v<char, typelist> << nl;
    stream << "is bool in the list: " << tft::is_type_in_list_v<bool, typelist> << nl;

}

int main()
{
    test_unique_type();
}