#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void motivation()
{
    int a = 5;

    std::cout << Tpf_GetTypeCategory(a) << std::endl;

    using type_t = double;

    std::cout << Tpf_GetTypeName(type_t) << std::endl;

    using type_list = tpf::type_list_t<char, int, double>;

    // std::cout << type_list{} << std::endl;

    stream << type_list{} << nl;
  
}

// it takes zero or more types
template<typename... Types>
struct type_list_st{ };

// type alias for struct type_list_st<Types...>
template<typename... Types>
using type_list_t = type_list_st<Types...>;

// an instance of type_list_t<Types...>
template<typename... Types>
constexpr auto type_list_v = type_list_t<Types...>{};

template<size_t StartIndex, size_t EndIndex>
struct static_loop_st
{
    template<typename Type, typename... Types>
    static void print_type_list(std::ostream& os, const type_list_t<Type, Types...>&)
    {
        if constexpr( StartIndex < EndIndex)
        {
            os << Tpf_GetTypeName(Type);

            os << (StartIndex+1 != EndIndex ? ", ": "");

            static_loop_st<StartIndex+1, EndIndex>::template 
                print_type_list(os, type_list_t<Types...>{});
        }
    }

    static void print_type_list(std::ostream& os, const type_list_t<>&) { }

};

template<typename... Types>
std::ostream& operator<<(std::ostream& os, const type_list_t<Types...>& tl)
{
    os << "<";

    static_loop_st<0, sizeof...(Types)>::template print_type_list(os, tl);

    os << ">";

    return os;
}

void test_output_operator()
{
    using typelist1 = type_list_t<char, int, double>;

    std::cout << typelist1{} << std::endl;

    std::cout << "Empty list: " << type_list_v<> << std::endl;
    std::cout << type_list_v<int> << std::endl;
    std::cout << type_list_v<int, double> << std::endl;
    

}

int main()
{
    // motivation();
    test_output_operator();
    
}