#include "pch.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

/*
    When the arguments match both the Primary template
    and the specializations, the specialization is preferred.

    When the arguments match two or more specializations
    then the best matched specialization is preferred.
*/
template<typename... Types> // template parameter clause
struct type_count_st;   // this struct does not have definition body
                        // this is declaration as well as the primary template of type_count_st
                        // remember this is "Primary Template"
                        // you can provide default type parameter, 
                        // I will talk about default type parameter very soon.
                        // primary template determines the count of template parameter
                        // in this case, typename... Types are parameter pack
                        // typename... Types means ZERO or more types

// now specialization
template<> // template<> means this struct does not have any template parameter
struct type_count_st<> // <> is for specialization
{                      // since the primary template can take ZERO or more types
                       // empty parameter list <> is allowed
    static constexpr size_t value = 0;
};

// more specialization
template<typename Type, typename... Types> // NOTE that the parameter clause of specialization
                                            // can be different from that of primary template
                                            // Type, Types... means ONE or more
                                            //
                                            // But the argument count of the specialization
                                            // should be equal to the template parameter count
                                            // of the primary template

struct type_count_st<Type, Types...> // since this type_count_st is a specialization
{                                    // we have to provide type arguments such as Type, Types...
                                    
                                    // recursive type function call
    static constexpr size_t value = 1 + type_count_st<Types...>::value;
};

template< template<typename...> class TmpType, // one template template parameter
    typename... Types // ZERO or more types, or parameter pack
    > 
struct type_count_st< TmpType<Types...> > // specialization for template template parameter
                                          // this TmpType template tempate class
                                          // can take ZERO or more types, TmpType<Types...>
{
    static constexpr size_t value = type_count_st<Types...>::value;
};


// typename... Types means ZERO or more types
// so empty type_count_v<> is allowed
template<typename... Types>
constexpr auto type_count_v = type_count_st<Types...>::value;

// primary template
template<typename T>
struct is_template_st
{
    using type = T;
    static constexpr bool value = false;
};

// specialization
template< template<typename...> class TmpType, typename... Types>
struct is_template_st< TmpType<Types...> >
{
    using type = TmpType<Types...>;
    static constexpr bool value = true;
};

template<typename T>
constexpr auto is_template_v = is_template_st<T>::value;

// this primary template takes one template parameter
template<typename T>
struct element_type_st
{
    using type = T;
};

template< template<typename, typename...> class TmpType, 
          typename Type, typename... Types>
struct element_type_st< TmpType<Type, Types...> > // since the primary template class
                                                 // takes ONE template parameter
                                                // this specialization has to provide ONE argument
                                                // TmpType<Type, Types...> is ONE argument to the 
                                                // struct element_type_st <  TmpType<Type, Types...>  >
{
    using type = Type;
};

// this type function extract the first element type
// of the template class
template<typename T>
using element_type_t = typename element_type_st<T>::type;


int main()
{
    stream << "type_count_v<> : " << type_count_v<>  << nl;

    stream << "type_count_v<int>: " << type_count_v<int> << nl;

    stream << "type_count_v<short, int, long long, double> : " 
            << type_count_v<short, int, long long, double> << nl;

    stream << "std::vector<int>'s template parameter count: " 
        << type_count_v< std::vector<int> > 
        << nl;

    stream << "std::vector<int>'s real type name: " 
        << Tpf_GetTypeName( std::vector<int> )
        << nl;

    stream.std() << std::boolalpha;
    stream << "Is \"int\" a template ? " << is_template_v<int> << nl;

    stream << "Is \"std::vector<int>\" a template ? "
        << is_template_v< std::vector<int> > << nl;

    using ele_type_of_int_t = element_type_t<int>; // corresponds to the primary template of element_type_st

                                        // corresponds to the template specialization of element_type_st 
    using ele_type_of_vector_double_t = element_type_t < std::vector<double> >;

    stream << "The element type of \"int\" is " 
        << Tpf_GetTypeName(ele_type_of_int_t) << nl;

    stream << "The element type of \"std::vector<double>\" is "
        << Tpf_GetTypeName(ele_type_of_vector_double_t) << nl;


} 