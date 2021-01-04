#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

void motivation()
{
    // enumerators such as idx_age, idx_weight, idx_name are
    // compile-time expressions, or constant expressions
    enum : size_t { idx_age, idx_weight, idx_name };

    using item_t = std::variant<int, double, std::string>;
    using container_t = std::vector<item_t>;

    // a vector of variant
    container_t mybag { 1, 2, 3.14, "James", "Sophia", 30.5, "Good Man"};

    for(auto& e: mybag)
    {
        // e is a type of variant,
        // e.index() returns currently selected index of the variant
        // e.index() is a run-time variable
        switch(e.index())
        {
            case idx_age:
                stream << "Age    : " << std::get<idx_age>(e) << nl; break;

            case idx_weight:
                stream << "Weight : " << std::get<idx_weight>(e) << nl; break;

            case idx_name:
                stream << "Name   : " << std::get<idx_name>(e) << nl; break;
        }
    }

    /*
        In some other part of the code in the same function, for example,
        we may need to access each element of the vector of the variant,
        in such cases, we have to repeat the same switch statements.
        So, it is error-prone as well as unwieldy.
    */
}

///////////////
void better_approach()
{
    // enumerators such as idx_age, idx_weight, idx_name are
    // compile-time expressions, or constant expressions
    enum : size_t { idx_age, idx_weight, idx_name };

    using item_t = std::variant<int, double, std::string>;
    using container_t = std::vector<item_t>;

    // a vector of variant
    container_t mybag { 1, 2, 3.14, "James", "Sophia", 30.5, "Good Man"};

    // we cannot catch global object in the lambda capture clause.
    auto& stream = ::stream;
    auto& nl = ::nl;

    auto process = [&stream, &nl](auto& value, auto index)
    {
        switch( index )
        {
            case idx_age:
                stream << "Age    : " << value << nl; break;

            case idx_weight:
                stream << "Weight : " << value << nl; break;

            case idx_name:
                stream << "Name   : " << value << nl; break;
        }

    }; // do not forget semicolon after lambda definition

    for(auto& e: mybag)
        tpf::process_variant(e, process);

    /*
        The primary benefits of this approach are 
        1. we don't need to access to each element using std::get<index>(var) syntax
        2. we can use the lambda multiple times in the different part of the code 
    */
        
}

template<size_t StartIndex, size_t EndIndex>
struct static_loop_st
{
    template<typename VariantType, typename CallbackType>
    static void process_variant(const VariantType& var, CallbackType&& callback)
    {
        if constexpr( StartIndex < EndIndex )
        {
            if( StartIndex == var.index() )
            {
                callback(std::get<StartIndex>(var), // [](auto& value,
                    StartIndex                      //                auto index)
                    );
            }
            else
            {
                static_loop_st<StartIndex+1, EndIndex>::template 
                    process_variant(var, std::forward<CallbackType>(callback));
            }

        }
    }
};

template<typename CallbackType, typename Type, typename... Types>
void process_variant(const std::variant<Type, Types...>& var, CallbackType&& callback)
{
    static_loop_st<0, sizeof...(Types)+1>:: template
        process_variant(var, std::forward<CallbackType>(callback));
}

///////////////
void test_process_variant()
{
    // enumerators such as idx_age, idx_weight, idx_name are
    // compile-time expressions, or constant expressions
    enum : size_t { idx_age, idx_weight, idx_name };

    using item_t = std::variant<int, double, std::string>;
    using container_t = std::vector<item_t>;

    // a vector of variant
    container_t mybag { 1, 2, 3.14, "James", "Sophia", 30.5, "Good Man"};

    // we cannot catch global object in the lambda capture clause.
    auto& stream = ::stream;
    auto& nl = ::nl;

    auto process = [&stream, &nl](auto& value, auto index)
    {
        switch( index )
        {
            case idx_age:
                stream << "Age    : " << value << nl; break;

            case idx_weight:
                stream << "Weight : " << value << nl; break;

            case idx_name:
                stream << "Name   : " << value << nl; break;
        }

    }; // do not forget semicolon after lambda definition

    for(auto& e: mybag)
        process_variant(e, process);        
}

int main()
{
    // better_approach();
    test_process_variant();
}