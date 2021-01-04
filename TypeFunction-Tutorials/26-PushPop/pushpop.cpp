#include "pch.h"

#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void test_push_pop()
{
    using empty_list_t = tpf::type_list_t<>;

    using pop_front_empty = tft::pop_front_type_t<empty_list_t>;
    stream << "pop_front_empty: " << pop_front_empty{} << nl;

    using pop_back_empty = tft::pop_back_type_t<empty_list_t>;
    stream << "pop_back_empty: " << pop_back_empty{} << nl;

    using typelist = tpf::type_list_t<char, int, double>;

    stream <<"\nOriginal type list: " << typelist{} << nl;

    using push_front = tft::push_front_type_t<bool, typelist>;
    stream << "push_front: " << push_front{} << nl;

    using push_back = tft::push_back_type_t<bool, typelist>;
    stream << "push_back: " << push_back{} << nl;

    using pop_front = tft::pop_front_type_t<typelist>;
    stream << "pop_front: " << pop_front{} << nl;

    using pop_back = tft::pop_back_type_t<typelist>;
    stream << "pop_back: " << pop_back{} << nl;

}

int main()
{
   test_push_pop();
}