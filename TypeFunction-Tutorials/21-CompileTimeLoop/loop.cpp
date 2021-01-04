#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void test_output_operator_for_tuple_variant()
{
    using tuple_t = std::tuple<int, double, std::string, std::wstring>;

    tuple_t t {2, 3.14, "I love it", L"Wide String" };

    stream << "t = " << t << nl;

    using variant_t = std::variant<int, double, std::string>;

    variant_t v;

    v = 3;

    stream << "v = " << v << nl;

    v = 3.14;

    stream << "v = " << v << nl;

    v = "this is variant";

    stream << "v = " << v << nl;
}


template<size_t StartIndex, size_t EndIndex>
struct compile_time_loop_st
{
    static void remember_compile_time_loop(const char* msg)
    {
        if constexpr( StartIndex < EndIndex)
        {
            // TO DO: put your logic in this section
            stream << "Index " << StartIndex << " - " << msg << nl << nl;

            // this does not work, because StartIndex, and EndIndex
            // these are compile-time constant... these cannot be modified
            // compile_time_loop_st< ++StartIndex, EndIndex>::remember_compile_time_loop(msg);
            compile_time_loop_st< StartIndex+1, EndIndex>::remember_compile_time_loop(msg);
        }
    }


    template<typename TupleType>
    static void print_tuple(std::ostream& os, const TupleType& t)
    {
        if constexpr ( StartIndex < EndIndex)
        {
            const auto& v = std::get<StartIndex>(t);

            os << v;

            os << (StartIndex + 1 != EndIndex  ? ", ": "") ;
            
            // recursion
            compile_time_loop_st<StartIndex+1, EndIndex>::template print_tuple(os, t);
        }
    }

    template<typename TupleType>
    static void print_tuple(std::wostream& os, const TupleType& t)
    {
        if constexpr ( StartIndex < EndIndex)
        {
            const auto& v = std::get<StartIndex>(t);

            os << v;

            os << (StartIndex + 1 != EndIndex  ? L", ": L"") ;
            
            // recursion
            compile_time_loop_st<StartIndex+1, EndIndex>::template print_tuple(os, t);
        }
    }

    template<typename VariantType>
    static void print_variant(std::ostream& os, const VariantType& var)
    {
        if constexpr (StartIndex < EndIndex)
        {
            // StartIndex is compile-time index
            // var.index() is run-time index - currently selected item's index
            if( StartIndex == var.index() )
            {
                const auto& v = std::get<StartIndex>(var);
                os << v ;
            }
            else
            {
                // recursion
                compile_time_loop_st<StartIndex+1, EndIndex>::template print_variant(os, var);
            }
        }
    }

};

// in case of std::tuple<typename...>
// that is, std::tuple's parameter count is zero or more
template<typename... Types>
std::ostream& operator<<(std::ostream& os, const std::tuple<Types...>& tpl)
{
    os << "{";

    compile_time_loop_st<0, sizeof...(Types)>::template print_tuple(os, tpl);

    os << "}";

    return os;
}

// in case of std::tuple<typename...>
// that is, std::tuple's parameter count is zero or more
template<typename... Types>
std::wostream& operator<<(std::wostream& os, const std::tuple<Types...>& tpl)
{
    os << L"{";

    compile_time_loop_st<0, sizeof...(Types)>::template print_tuple(os, tpl);

    os << L"}";

    return os;
}

// in case of std::variant<typename, typename...>
// the parameter count of std::variant is 1 or more
template<typename Type, typename... Types>
std::ostream& operator<<(std::ostream& os, const std::variant<Type, Types...>& var)
{
    compile_time_loop_st<0, sizeof...(Types) + 1>:: template print_variant(os, var);

    return os;
}

void test_compile_time_loop()
{
    // iterates 0, 1, 2, 3, 4 -- 5 times
    compile_time_loop_st<0, 5>::remember_compile_time_loop(
            "This is a compile-time loop.\n\t\t"
            "I coined \"compile-time\" loop!");
}

void test_std_output_operator_for_tuple_variant()
{
    using tuple_t = std::tuple<int, double, std::wstring>;

    // using tuple_t = std::tuple<int, double, std::wstring, std::string>;

    //tuple_t t {2, 3.14, L"Wide String", "byte string"};
    tuple_t t {2, 3.14, L"Wide String"};

    std::wcout << "t = " << t << std::endl;

    using variant_t = std::variant<int, double, std::string>;

    variant_t v;

    v = 3;

    std::cout << "v = " << v << std::endl;

    v = 3.14;

    
    std::cout << "v = " << v << std::endl;

    v = "this is variant";

    
    std::cout << "v = " << v << std::endl;

}


int main()
{
    // test_output_operator_for_tuple_variant();
    // test_compile_time_loop();
    test_std_output_operator_for_tuple_variant();
}