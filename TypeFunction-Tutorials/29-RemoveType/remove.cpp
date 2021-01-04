#include "pch.h"

#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void test_remove_type()
{
   using typelist = tpf::type_list_t<char, int, float, int, double, int>;
   
   // remove int from typelist
   using removed_t = tft::remove_type_t<int, typelist>;

   stream << "int is removed from " << typelist{} << ", the result: " << removed_t{} << nl;

}

void test_difference_type()
{
   using typelist1 = tpf::type_list_t<char, int, float, double>;
   using typelist2 = tpf::type_list_t<char, float>;

   using differ_types = tft::difference_type_t<typelist1, typelist2>;

   stream << typelist1{} << " - " << typelist2{} << " = " << differ_types{} << nl;
}

int main()
{
   //  test_remove_type();
   test_difference_type();
}