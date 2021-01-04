#include "pch.h"

#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void test_replace_type()
{
   using typelist = tpf::type_list_t<char, int, double>;

   // replace all occurrences of char with bool in the typelist
   using adjusted = tft::replace_type_t<bool, char, typelist>;

   stream << "Type list: "<< typelist{} << ", Adjusted List: " << adjusted{} << nl;

   using typelist2 = tpf::type_list_t<const char*, int, double, const char*>;
   using char_to_string = tft::replace_type_t<std::string, const char*, typelist2>;

   stream << "\nType list: "<< typelist2{} << ", char_to_string: " << char_to_string{} << nl;

}

int main()
{
  test_replace_type();
}