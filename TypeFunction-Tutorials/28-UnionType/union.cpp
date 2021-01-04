#include "pch.h"

#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void test_union_type()
{
   using list1 = tpf::type_list_t<char, double>;
   using list2 = tpf::type_list_t<int, float>;

   using union_t = tft::union_type_t<list1, list2>;

   stream << list1{} << " + " << list2{} << " = " << union_t{} << nl;
}

void test_intersection_type()
{
   using list1 = tpf::type_list_t<char, int, double, float>;
   using list2 = tpf::type_list_t<int, float, bool, double>;

   using intersection_t = tft::intersection_type_t<list1, list2>;

   stream << list1{} << " * " << list2{} << " = " << intersection_t{} << nl;
}

int main()
{
   test_union_type();
   // test_intersection_type();
}