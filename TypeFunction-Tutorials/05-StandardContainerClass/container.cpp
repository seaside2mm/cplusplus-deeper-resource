#include "pch.h"
#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

/*
    There are two ways to pass container class to a function.
*/

// simpler way
template<typename CntrType>
void print_container(const CntrType& v)
{
    // for(const auto& e: v)
    // {
    //     stream << e << ", ";
    // }

    stream << v << nl;
}

// advanced way, but it does not work
// template< template<typename> class CntrType, typename Type>
// void pass_template_container_advanced_way(const CntrType<Type>& cntr)
// {
//     stream << cntr << nl;
// }

// advanced proper way to pass Standard C++ container class as function argument
template< template<typename, typename...> class CntrType,
            typename Type, typename... Types>
void pass_containers_properly(const CntrType<Type, Types...>& cntr)
{
    stream << cntr << nl;
}

// the proper way to reverse container elements
template< template<typename, typename...> class CntrType,
        typename Type, typename... Types>
auto reversed_elements(const CntrType<Type, Types...>& cntr)
{
    using remove_const_type_t = tft::remove_const_reference_t<Type>;
    
    std::vector<std::reference_wrapper<const remove_const_type_t>> 
        reversed_cntr{cntr.crbegin(), cntr.crend() };

    return reversed_cntr;
}

// the proper way to reverse container elements
template< template<typename, typename...> class ReturnCntrType = std::vector,
template<typename, typename...> class CntrType,
        typename Type, typename... Types>
auto reversed_element_order(const CntrType<Type, Types...>& cntr)
{
    using remove_const_type_t = tft::remove_const_reference_t<Type>;
    
    ReturnCntrType<std::reference_wrapper<const remove_const_type_t>> 
        reversed_cntr{cntr.crbegin(), cntr.crend() };

    return reversed_cntr;
}

int main()
{
    stream << "Template parameter count : "
        << tft::type_count_v< std::vector<int> > << nl;

    stream.std() << std::boolalpha;

    stream << "Is std::vector<int> a container? " 
        << tft::is_container_v < std::vector<int> > << nl;

    stream << "Is std::list<int> a container ? "
        << tft::is_container_v < std::list<int> > << nl;

    stream << "Is std::unordered_multimap<std::string, int> a container ? "
        << tft::is_container_v < std::unordered_multimap<std::string, int> > << nl;

    std::vector<int> ints{1, 2, 3, 4, 5};

    // print_container(ints);

    // pass_template_container_advanced_way(ints);

    pass_containers_properly(ints);

    //auto reversed = reversed_elements(ints);

    // auto reversed = reversed_element_order(ints);

    auto reversed = tft::reversed_elements_order(ints);
    
    stream << "Type of reversed: " << Tpf_GetTypeCategory(reversed) << nl;
    stream << "Reversed Elements: " << reversed << nl;
    
}