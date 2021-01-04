#include "pch.h"

#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void test_select_first_type()
{
    using type0_t = tft::select_first_type_t<>;
    stream << "type0_t: " << tpf::type_list_v<type0_t> << nl;

    using type1_t = tft::select_first_type_t<char>;
    stream << "type1_t: " << tpf::type_list_v<type1_t> << nl;

    using type2_t = tft::select_first_type_t<char, int>;
    stream << "type2_t: " << tpf::type_list_v<type2_t> << nl;

    /////////////////////////////////////
    using typelist0 = tpf::type_list_t<>;
    using first_type0 = tft::select_first_type_t<typelist0>;
    stream << "Select first type out of " << typelist0{} 
        << " : " << tpf::type_list_v<first_type0> << nl;

    using typelist1 = tpf::type_list_t<char>;
    using first_type1 = tft::select_first_type_t<typelist1>;
    stream << "Select first type out of " << typelist1{} 
        << " : " << tpf::type_list_v<first_type1> << nl;

    using typelist2 = tpf::type_list_t<char, int>;
    using first_type2 = tft::select_first_type_t<typelist2>;
    stream << "Select first type out of " << typelist2{} 
        << " : " << tpf::type_list_v<first_type2> << nl;

}

void test_select_last_type()
{
    using type0_t = tft::select_last_type_t<>;
    stream << "type0_t: " << tpf::type_list_v<type0_t> << nl;

    using type1_t = tft::select_last_type_t<char>;
    stream << "type1_t: " << tpf::type_list_v<type1_t> << nl;

    using type2_t = tft::select_last_type_t<char, int, double>;
    stream << "type2_t: " << tpf::type_list_v<type2_t> << nl;

    //////////////////////////////////////////////////////
    using typelist0 = tpf::type_list_t<>;
    using last0_t = tft::select_last_type_t<typelist0>;
    stream << "typelist0: " << typelist0{} <<" : " << tpf::type_list_v<last0_t> << nl;

    using typelist1 = tpf::type_list_t<char>;
    using last1_t = tft::select_last_type_t<typelist1>;
    stream << "typelist1: " << typelist1{} <<" : " << tpf::type_list_v<last1_t> << nl;

    using typelist2 = tpf::type_list_t<char, int, double>;
    using last2_t = tft::select_last_type_t<typelist2>;
    stream << "typelist2: " << typelist2{} <<" : " << tpf::type_list_v<last2_t> << nl;

}

void test_select_n_th_type()
{
    using type0_t = tft::select_n_th_type_t<0>;
    stream << "type0_t: " << tpf::type_list_v<type0_t> << nl;

    using type1_t = tft::select_n_th_type_t<4>;
    stream << "type1_t: " << tpf::type_list_v<type1_t> << nl;

    using n_th_0 = tft::select_n_th_type_t<0, char, int, double>;
    stream << "n_th_0: " << tpf::type_list_v<n_th_0> << nl;

    using n_th_1 = tft::select_n_th_type_t<1, char, int, double>;
    stream << "n_th_1: " << tpf::type_list_v<n_th_1> << nl;

    using n_th_2 = tft::select_n_th_type_t<2, char, int, double>;
    stream << "n_th_2: " << tpf::type_list_v<n_th_2> << nl;

    using n_th_3 = tft::select_n_th_type_t<3, char, int, double>;
    stream << "n_th_3: " << tpf::type_list_v<n_th_3> << nl;

    /////////////////////////////////////////////////////////

    using typelist0 = tpf::type_list_t<>;
    using nth_0 = tft::select_n_th_type_t<0>;
    stream << "nth_0 : " << tpf::type_list_v<nth_0> << nl;

    using nth_1 = tft::select_n_th_type_t<5>;
    stream << "nth_1 : " << tpf::type_list_v<nth_1> << nl;

    using typelist_t = tpf::type_list_t<char, int, double>;

    using n_0 = tft::select_n_th_type_t<0, typelist_t>;
    stream <<"n_0 : " << tpf::type_list_v<n_0> << nl;

    using n_1 = tft::select_n_th_type_t<1, typelist_t>;
    stream <<"n_1 : " << tpf::type_list_v<n_1> << nl;
    
    using n_2 = tft::select_n_th_type_t<2, typelist_t>;
    stream <<"n_2 : " << tpf::type_list_v<n_2> << nl;
    
    using n_3 = tft::select_n_th_type_t<3, typelist_t>;
    stream <<"n_3 : " << tpf::type_list_v<n_3> << nl;
    
}

int main()
{
   // test_select_first_type();
   // test_select_last_type();
   test_select_n_th_type();
}