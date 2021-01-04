#ifndef _TFT_TYPE_LIB_H
#define _TFT_TYPE_LIB_H

#include "tpf_type_util.h"

namespace tft
{
    template<typename T>
    using remove_const_reference_t = std::remove_const_t<std::remove_reference_t<T>>;

    /*
    When the arguments match both the Primary template
    and the specializations, the specialization is preferred.

    When the arguments match two or more specializations
    then the best matched specialization is preferred.
    */
    namespace hide
    {
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

    } // end of namespace hide

    // typename... Types means ZERO or more types
    // so empty type_count_v<> is allowed
    template<typename... Types>
    constexpr auto type_count_v = hide::type_count_st<Types...>::value;

    namespace hide
    {
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
    } // end of namespace hide

    template<typename T>
    constexpr auto is_template_v = hide::is_template_st<T>::value;

    namespace hide
    {
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
        
    } // end of namespace hide

    // this type function extract the first element type
    // of the template class
    template<typename T>
    using element_type_t = typename hide::element_type_st<T>::type;

    namespace hide
    {
        // primary template
        template<typename T> // primary template requires ONE parameter
        struct is_container_st
        {
            static constexpr bool value = false;
        }; 

        template<typename Type, typename... Types>
        struct is_container_st<std::vector<Type, Types...>>
        {
            static constexpr bool value = true;
        };

        template<typename Type, typename... Types>
        struct is_container_st<std::deque<Type, Types...>>
        {
            static constexpr bool value = true;
        };
        
        template<typename Type, typename... Types>
        struct is_container_st<std::set<Type, Types...>>
        {
            static constexpr bool value = true;
        };

        template<typename Type, typename... Types>
        struct is_container_st<std::multiset<Type, Types...>>
        {
            static constexpr bool value = true;
        };

        template<typename Type, typename... Types>
        struct is_container_st<std::unordered_set<Type, Types...>>
        {
            static constexpr bool value = true;
        };

        template<typename Type, typename... Types>
        struct is_container_st<std::unordered_multiset<Type, Types...>>
        {
            static constexpr bool value = true;
        };

        template<typename Type, typename... Types>
        struct is_container_st<std::list<Type, Types...>>
        {
            static constexpr bool value = true;
        };

        template<typename KeyType, typename ValueType, typename... Types>
        struct is_container_st<std::map<KeyType, ValueType, Types...>>
        {
            static constexpr bool value = true;
        };

        template<typename KeyType, typename ValueType, typename... Types>
        struct is_container_st<std::multimap<KeyType, ValueType, Types...>>
        {
            static constexpr bool value = true;
        };

        template<typename KeyType, typename ValueType, typename... Types>
        struct is_container_st<std::unordered_map<KeyType, ValueType, Types...>>
        {
            static constexpr bool value = true;
        };

        template<typename KeyType, typename ValueType, typename... Types>
        struct is_container_st<std::unordered_multimap<KeyType, ValueType, Types...>>
        {
            static constexpr bool value = true;
        };

    } // end of namespace hide

    template<typename T>
    constexpr auto is_container_v = hide::is_container_st<T>::value;

    // far advanced way
    template< template<typename, typename...> class ReturnCntrType = std::vector, typename CntrType>
    auto reversed_elements_order(const CntrType& cntr)
    {
        using ele_t = tft::element_type_t<CntrType>;
        
        ReturnCntrType<std::reference_wrapper<const ele_t>> 
            reversed_cntr{cntr.crbegin(), cntr.crend() };

        return reversed_cntr;
    }

    struct NoTypeDummy { };

    namespace hide
    {
        // this is a function that does not have its function body
        // true ? int{} : double{} ... this ternary operator ?: returns a pure rvalue
        // we remove const_reference from this value
        // this function gets called only when "true ? s : t" succeeds.
        template<typename S, typename T>
        auto common_type_func(S&& s, T&& t) -> remove_const_reference_t<decltype(true ? s : t)> ;
        // catch all function
        NoTypeDummy common_type_func(...);

        // std::declval<T>() is defined as below
        //
        // template<typename T> 
        // std::remove_reference_t<T>&& declval() noexcept;
        //
        // returns an rvalue reference T&&
        template<typename S, typename T>
        using common_type_of_s_and_t = decltype(common_type_func( std::declval<S>(), std::declval<T>()));

    } // end of namespace hide

    template<typename S, typename T>
    constexpr auto exist_common_type_v = !std::is_same_v<NoTypeDummy, hide::common_type_of_s_and_t<S, T>>;

    template<typename S, typename T>
    constexpr auto are_types_operable_v = exist_common_type_v<S, T>;

    namespace hide
    {
        // primary class template determines the template argument counts
        // <typename T, typename... Types> means common_type_st takes one OR more of arguments
        template<typename T, typename... Types>
        struct common_type_st;

        // partial specialization, taking one argument
        template<typename T>
        struct common_type_st<T>
        {
            using type = T;
        };

        // partial specialization, taking two arguments
        template<typename S, typename T>
        struct common_type_st<S, T>
        {
            using type = common_type_of_s_and_t<S, T>;
        };

        // partial specialization, taking two or more of arguments
        // this is the toughest part of this type function
        template<typename S, typename T, typename... Types>
        struct common_type_st<S, T, Types...>
        {
            // we will be using recursion all-the-time
            // please make yourself familiar with recursion
            using type = common_type_of_s_and_t< 
                typename common_type_st<S, T>::type,
                typename common_type_st<T, Types...>::type >;
        };

        // tpf::type_list_t<T> is defined as below
        //
        // template<typename... Types> type_list_t<Types...>
        //
        // partial specialization for common_type_st< tpf::type_list_t<T> >
        template<typename T>
        struct common_type_st< tpf::type_list_t<T> >
        {
            using type = T;
        };

        template<typename S, typename T>
        struct common_type_st< tpf::type_list_t<S, T> >
        {
            using type = typename common_type_st<S, T>::type;
        };

        template<typename S, typename T, typename... Types>
        struct common_type_st<tpf::type_list_t<S, T, Types...>>
        {
            using type = typename common_type_st<S, T, Types...>::type;
        };

    } // end of namespace hide

    // this type function returns common type of T, Types...
    // if fails, it returns NoTypeDummy
    template<typename T, typename... Types>
    using common_type_t = typename hide::common_type_st<T, Types...>::type;

    // this function returns true if common type exists
    // otherwise, it returns false
    template<typename T, typename... Types>
    constexpr auto common_type_v = !std::is_same_v<NoTypeDummy, common_type_t<T, Types...>>;

    // this function returns common type wrapped in tpf::type_list_t<common_type>
    // if fails, it returns tpf::type_list_t<NoTypeDummy>
    template<typename T, typename... Types>
    using common_typelist_t = tpf::type_list_t<common_type_t<T, Types...>>;

    // this function returns an instance of tpf::type_list_t<common_type>
    // it is for easy display of the common type
    template<typename T, typename... Types>
    constexpr auto common_typelist_v = common_typelist_t<T, Types...>{};
    
    template<typename T>
    constexpr auto is_integer_v = !std::is_same_v<bool, remove_const_reference_t<T>> 
        && std::is_integral_v<remove_const_reference_t<T>> 
        && std::is_arithmetic_v<remove_const_reference_t<T>>;

    namespace hide
    {
        // this function does not work
        // template<typename T>
        // using make_signed_t = std::conditional_t< tft::is_integer_v<T>, // if true
        //     std::make_signed_t<T>, // make T signed integer
        //      T // otherwise return T
        //      >;

        // primary class template determines the template argument count
        template<typename T, bool bInteger=false>
        struct make_signed_st // <T, false> -- false for non-integral type
        {
            using type = T;
        };

        template<typename T>
        struct make_signed_st<T, true> // <T, true> - T is an integral type
        {
            using type = std::make_signed_t<T>;
        };

        template<typename T, typename... Types>
        using signed_common_type_t = typename make_signed_st<
            common_type_t<T, Types...>, 
            is_integer_v<common_type_t<T, Types...>>>::type;

    } // end of namespace hide

    template<typename T, typename... Types>
    using signed_common_type_t = hide::signed_common_type_t< 
        remove_const_reference_t<T>, remove_const_reference_t<Types>...>;

    namespace hide
    {
        template<typename... Types>
        struct select_first_type_st;

        template<>
        struct select_first_type_st<>
        {
            using type = NoTypeDummy;
            using typelist = tpf::type_list_t<>;
        };

        template<typename Type>
        struct select_first_type_st<Type>
        {
            using type = Type;
             using typelist = tpf::type_list_t<Type>;
        };

        template<typename Type, typename... Types>
        struct select_first_type_st<Type, Types...>
        {
            using type = Type;
             using typelist = tpf::type_list_t<Type>;
        };

        /////////////////////////////////////
        template<>
        struct select_first_type_st<tpf::type_list_t<>>
        {
            using type = NoTypeDummy;
             using typelist = tpf::type_list_t<>;
        };

        template<typename Type>
        struct select_first_type_st<tpf::type_list_t<Type>>
        {
            using type = Type;
             using typelist = tpf::type_list_t<Type>;
        };

        template<typename Type, typename... Types>
        struct select_first_type_st<tpf::type_list_t<Type, Types...>>
        {
            using type = Type;
             using typelist = tpf::type_list_t<Type>;
        };

    } // end of namespace hide

    template<typename... Types>
    using select_first_type_t = typename hide::select_first_type_st<Types...>::type;

    template<typename... Types>
    using select_first_type_list_t = typename hide::select_first_type_st<Types...>::typelist;

    namespace hide
    {
        template<typename... Types>
        struct select_last_type_st;

        template<>
        struct select_last_type_st<>
        {
            using type = NoTypeDummy;

            using typelist = tpf::type_list_t<>;
        };

        template<typename Type>
        struct select_last_type_st<Type>
        {
            using type = Type;
            using typelist = tpf::type_list_t<type>;
        };

        template<typename Type, typename... Types>
        struct select_last_type_st<Type, Types...>
        {
            using type = std::conditional_t< sizeof...(Types) == 0,
                Type, typename select_last_type_st<Types...>::type>;

            using typelist = tpf::type_list_t<type>;
        };

        ////////////////////////////////
        template<>
        struct select_last_type_st<tpf::type_list_t<>>
        {
            using type = NoTypeDummy;

            using typelist = tpf::type_list_t<>;
        };

        template<typename Type>
        struct select_last_type_st<tpf::type_list_t<Type>>
        {
            using type = Type;

            using typelist = tpf::type_list_t<type>;
        };

        template<typename Type, typename... Types>
        struct select_last_type_st<tpf::type_list_t<Type, Types...>>
        {
            using type = typename select_last_type_st<Type, Types...>::type;

            using typelist = tpf::type_list_t<type>;
        };

    } // end of namespace hide

    template<typename... Types>
    using select_last_type_t = typename hide::select_last_type_st<Types...>::type;

    template<typename... Types>
    using select_last_type_list_t = typename hide::select_last_type_st<Types...>::typelist;

    namespace hide
    {
        template<auto SelectIndex, typename... Types>
        struct select_n_th_type_st;

        template<auto SelectIndex>
        struct select_n_th_type_st<SelectIndex>
        {
            using type = NoTypeDummy;
            using typelist = tpf::type_list_t<>;
        };

        template<typename Type, typename... Types>
        struct select_n_th_type_st<0, Type, Types...>
        {
            using type = Type;
            using typelist = tpf::type_list_t<type>;
        };

        template<auto SelectIndex, typename Type, typename... Types>
        struct select_n_th_type_st<SelectIndex, Type, Types...>
        {
            using type = std::conditional_t< SelectIndex == 0,
                Type, typename select_n_th_type_st<SelectIndex-1, Types...>::type>;

            using typelist = tpf::type_list_t<type>;
        };

        //////////////////////////////////////////////////
         template<auto SelectIndex>
        struct select_n_th_type_st<SelectIndex, tpf::type_list_t<>>
        {
            using type = NoTypeDummy;
            using typelist = tpf::type_list_t<>;
        };

        template<typename Type, typename... Types>
        struct select_n_th_type_st<0, tpf::type_list_t< Type, Types...>>
        {
            using type = Type;
            using typelist = tpf::type_list_t<type>;
        };

        template<auto SelectIndex, typename Type, typename... Types>
        struct select_n_th_type_st<SelectIndex, tpf::type_list_t<Type, Types...>>
        {
            using type = typename select_n_th_type_st<SelectIndex, Type, Types...>::type;

            using typelist = tpf::type_list_t<type>;
        };
    } // end of namespace hide

    template<auto SelectIndex, typename... Types>
    using select_n_th_type_t = typename hide::select_n_th_type_st<SelectIndex, Types...>::type;

    template<auto SelectIndex, typename... Types>
    using select_n_th_type_list_t = typename hide::select_n_th_type_st<SelectIndex, Types...>::typelist;

    namespace hide
    {
        template<typename ArgType, typename ListType>
        struct push_front_type_st;

        template<typename ArgType, typename... Types>
        struct push_front_type_st<ArgType, tpf::type_list_t<Types...>>
        {
            using type = tpf::type_list_t<ArgType, Types...>;
        };

        template<typename ArgType, typename ListType>
        using push_front_type_t = typename push_front_type_st<ArgType, ListType>::type;

        template<typename ArgType, typename ListType>
        struct push_back_type_st;

        template<typename ArgType, typename... Types>
        struct push_back_type_st<ArgType, tpf::type_list_t<Types...>>
        {
            using type = tpf::type_list_t<Types..., ArgType>;
        };

        template<typename ArgType, typename ListType>
        using push_back_type_t = typename push_back_type_st<ArgType, ListType>::type;

        template<typename ListType>
        struct pop_front_type_st;

        template<>
        struct pop_front_type_st<tpf::type_list_t<>>
        {
            using type = tpf::type_list_t<>;
        };

        template<typename Type, typename... Types>
        struct pop_front_type_st<tpf::type_list_t<Type, Types...>>
        {
            using type = tpf::type_list_t<Types...>;
        };

        template<typename ListType>
        using pop_front_type_t = typename pop_front_type_st<ListType>::type;

        template<typename LeftList, typename RightList>
        struct pop_back_type_st;

        template<typename... LeftTypes>
        struct pop_back_type_st<tpf::type_list_t<LeftTypes...>, tpf::type_list_t<>>
        {
            using type = tpf::type_list_t<LeftTypes...>;
        };

        // template<typename... LeftTypes, typename Type>
        // struct pop_back_type_st<tpf::type_list_t<LeftTypes...>, tpf::type_list_t<Type>>
        // {
        //     using type = tpf::type_list_t<LeftTypes...>;
        // };

        template<typename... LeftTypes, typename Type, typename... RightTypes>
        struct pop_back_type_st<tpf::type_list_t<LeftTypes...>, tpf::type_list_t<Type, RightTypes...>>
        {
            using left_list = push_back_type_t<Type, tpf::type_list_t<LeftTypes...>>;
            using right_list = tpf::type_list_t<RightTypes...>;

            using type = std::conditional_t< sizeof...(RightTypes) == 0,
                tpf::type_list_t<LeftTypes...>, typename pop_back_type_st<left_list, right_list>::type >;
        };

        template<typename ListType>
        using pop_back_type_t = typename pop_back_type_st<tpf::type_list_t<>, ListType>::type;

    } // end of namespace hide

    template<typename ArgType, typename ListType>
    using push_front_type_t = hide::push_front_type_t<ArgType, ListType>;

    template<typename ArgType, typename ListType>
    using push_back_type_t = hide::push_back_type_t<ArgType, ListType>;

    template<typename ListType>
    using pop_front_type_t = hide::pop_front_type_t<ListType>;

    template<typename ListType>
    using pop_back_type_t = hide::pop_back_type_t<ListType>;

    namespace hide
    {
        template<typename ArgType, typename... Types>
        struct is_type_in_list_st;

        template<typename ArgType>
        struct is_type_in_list_st<ArgType>
        {
            static constexpr bool value = false;
        };

        template<typename ArgType, typename Type, typename... RightTypes>
        struct is_type_in_list_st<ArgType, Type, RightTypes...>
        {
            static constexpr bool value = std::is_same_v<ArgType, Type> ? true :
                is_type_in_list_st<ArgType, RightTypes...>::value;
        };

        template<typename ArgType>
        struct is_type_in_list_st<ArgType, tpf::type_list_t<>>
        {
            static constexpr bool value = false;
        };

        template<typename ArgType, typename Type, typename... RightTypes>
        struct is_type_in_list_st<ArgType, tpf::type_list_t<Type, RightTypes...>>
        {
            static constexpr bool value = is_type_in_list_st<ArgType, Type, RightTypes...>::value;
        };

        template<typename ArgType, typename... Types>
        constexpr auto is_type_in_list_v = is_type_in_list_st<ArgType, Types...>::value;

        template<typename ListType, typename... Types>
        struct unique_type_st;

        template<typename... Types>
        struct unique_type_st<tpf::type_list_t<Types...>>
        {
            using type = tpf::type_list_t<Types...>;
        };

        template<typename... Types, typename Type>
        struct unique_type_st<tpf::type_list_t<Types...>, Type>
        {
            using list = tpf::type_list_t<Types...>;
            using type = std::conditional_t< is_type_in_list_v<Type, list>,
                list, // is_type_in_list_v<Type, list> is true, then Type is already in the list
                push_back_type_t<Type, list>>;
        };

        template<typename... Types, typename Type, typename... RightTypes>
        struct unique_type_st<tpf::type_list_t<Types...>, Type, RightTypes...>
        {
            using list = typename unique_type_st<tpf::type_list_t<Types...>, Type>::type; 
            using type = typename unique_type_st<list, RightTypes...>::type;
        };

        template<typename... Types, typename... RightTypes>
        struct unique_type_st<tpf::type_list_t<Types...>, tpf::type_list_t<RightTypes...>>
        {
            using type = typename unique_type_st<tpf::type_list_t<Types...>, RightTypes...>::type;
        };
                
        template<typename... Types>
        using unique_type_t = typename unique_type_st<tpf::type_list_t<>, Types...>::type;

        template<typename LeftList, typename RightList>
        struct prepend_type_st;

        template<typename... ArgTypes, typename... RightTypes>
        struct prepend_type_st<tpf::type_list_t<ArgTypes...>, tpf::type_list_t<RightTypes...>>
        {
            using type = tpf::type_list_t<ArgTypes..., RightTypes...>;
        };

        template<typename ArgList, typename RightList>
        using prepend_type_t = typename prepend_type_st<ArgList, RightList>::type;

        template<typename LeftList, typename RightList>
        struct append_type_st;

        template<typename... ArgTypes, typename... RightTypes>
        struct append_type_st<tpf::type_list_t<ArgTypes...>, tpf::type_list_t<RightTypes...>>
        {
            using type = tpf::type_list_t<RightTypes..., ArgTypes...>;
        };

        template<typename ArgList, typename RightList>
        using append_type_t = typename append_type_st<ArgList, RightList>::type;

    } // end of namespace hide

    template<typename ArgType, typename... Types>
    constexpr auto is_type_in_list_v = hide::is_type_in_list_v<ArgType, Types...>;

    template<typename... Types>
    using unique_type_t = hide::unique_type_t<Types...>;

    template<typename ArgList, typename RightList>
    using prepend_type_t = hide::prepend_type_t<ArgList, RightList>;

    template<typename ArgList, typename RightList>
    using append_type_t = hide::append_type_t<ArgList, RightList>;

    namespace hide
    {
        template<typename LeftList, typename... Types>
        struct union_type_st;

        template<typename... Types>
        struct union_type_st<tpf::type_list_t<Types...>>
        {
            using type = tpf::type_list_t<Types...>;
        };

        template<typename... Types, typename Type>
        struct union_type_st<tpf::type_list_t<Types...>, Type>
        {
            using list = tpf::type_list_t<Types...>;

            using type = std::conditional_t< is_type_in_list_v<Type, list>,
            list, // if Type is already in the list, tpf::type_list_t<Types...>
                  // then we skip Type
            push_back_type_t<Type, list>>; 
        };

        template<typename... Types, typename Type, typename... RightTypes>
        struct union_type_st<tpf::type_list_t<Types...>, Type, RightTypes...>
        {
            using list = typename union_type_st<tpf::type_list_t<Types...>, Type>::type;

            using type = std::conditional_t<sizeof...(RightTypes) == 0,
                list,
                typename union_type_st<list, RightTypes...>::type>;
        };

        ///////////////////////////////////

        template<typename... Types>
        struct union_type_st<tpf::type_list_t<Types...>, tpf::type_list_t<>>
        {
            using type = tpf::type_list_t<Types...>;
        };

        template<typename... Types, typename Type>
        struct union_type_st<tpf::type_list_t<Types...>, tpf::type_list_t<Type>>
        {
            using type = typename union_type_st<tpf::type_list_t<Types...>, Type>::type;
        };

        template<typename... Types, typename Type, typename... RightTypes>
        struct union_type_st<tpf::type_list_t<Types...>, tpf::type_list_t<Type, RightTypes...>>
        {
            using type = 
                typename union_type_st<tpf::type_list_t<Types...>, Type, RightTypes...>::type;
        };

        template<typename LeftList, typename RightList>
        using union_type_t = typename union_type_st<LeftList, RightList>::type;

        template<typename LeftList, typename... Types>
        struct intersection_type_st;

        template<typename... Types>
        struct intersection_type_st<tpf::type_list_t<Types...>>
        {
            using type = tpf::type_list_t<Types...>;
        };

        template<typename... Types, typename Type>
        struct intersection_type_st<tpf::type_list_t<Types...>, Type>
        {
            using list = tpf::type_list_t<Types...>;

            using type = std::conditional_t<is_type_in_list_v<Type, list>,
                tpf::type_list_t<Type>, tpf::type_list_t<>>;
        };

        template<typename... Types, typename Type, typename... RightTypes>
        struct intersection_type_st<tpf::type_list_t<Types...>, Type, RightTypes...>
        {
            using left_list = tpf::type_list_t<Types...>;
            using list = typename intersection_type_st<left_list, Type>::type;

            using type = std::conditional_t<sizeof...(RightTypes) == 0,
                list, prepend_type_t<list, typename intersection_type_st<left_list, RightTypes...>::type>>;
        };

    ////////////////////////////////////////////////////////////
        template<typename... Types>
        struct intersection_type_st<tpf::type_list_t<Types...>, tpf::type_list_t<>>
        {
            using type = tpf::type_list_t<Types...>;
        };

        template<typename... Types, typename Type>
        struct intersection_type_st<tpf::type_list_t<Types...>, 
            tpf::type_list_t<Type>>
        {
            using type = typename intersection_type_st<tpf::type_list_t<Types...>, Type>::type;
        };

        template<typename... Types, typename Type, typename... RightTypes>
        struct intersection_type_st<tpf::type_list_t<Types...>, 
            tpf::type_list_t<Type, RightTypes...>>
        {
            using type = typename intersection_type_st<tpf::type_list_t<Types...>, 
                            Type, RightTypes...>::type;
        };

        template<typename LeftList, typename RightList>
        using intersection_type_t = typename intersection_type_st<LeftList, RightList>::type;

    } // end of namespace hide

    template<typename LeftList, typename RightList>
    using union_type_t = hide::union_type_t<unique_type_t<LeftList>, unique_type_t<RightList>>;

    template<typename LeftList, typename RightList>
    using intersection_type_t = hide::intersection_type_t<unique_type_t<LeftList>, unique_type_t<RightList>>;

    namespace hide
    {
        template<typename ArgType, typename... Types>
        struct remove_type_st;

        template<typename ArgType>
        struct remove_type_st<ArgType>
        {
            using type = tpf::type_list_t<>;
        };

        template<typename ArgType, typename Type>
        struct remove_type_st<ArgType, Type>
        {
            using type = std::conditional_t< std::is_same_v<ArgType, Type>,
                tpf::type_list_t<>, tpf::type_list_t<Type>>;
        };

        template<typename ArgType, typename Type, typename... RightTypes>
        struct remove_type_st<ArgType, Type, RightTypes...>
        {
            using list = typename remove_type_st<ArgType, Type>::type;

            using type = std::conditional_t< sizeof...(RightTypes) == 0,
                list, prepend_type_t<list, typename remove_type_st<ArgType, RightTypes...>::type> >;
        };

        ////////////////////////////////////////
        template<typename ArgType>
        struct remove_type_st<ArgType, tpf::type_list_t<>>
        {
            using type = tpf::type_list_t<>;
        };

        template<typename ArgType, typename Type>
        struct remove_type_st<ArgType, tpf::type_list_t<Type>>
        {
            using type =  typename remove_type_st<ArgType, Type>::type;
        };

        template<typename ArgType, typename Type, typename... RightTypes>
        struct remove_type_st<ArgType, tpf::type_list_t<Type, RightTypes...>>
        {
            using type = typename remove_type_st<ArgType, Type, RightTypes...>::type;
        };

        template<typename ArgType, typename TypeList>
        using remove_type_t = typename remove_type_st<ArgType, TypeList>::type;

        /////////////////////////////////
        template<typename LeftList, typename... Types>
        struct difference_type_st;

        template<typename... Types>
        struct difference_type_st<tpf::type_list_t<Types...>>
        {
            using type = tpf::type_list_t<Types...>;
        };

        template<typename... Types, typename Type>
        struct difference_type_st<tpf::type_list_t<Types...>, Type>
        {
            using type = remove_type_t<Type, tpf::type_list_t<Types...>>;
        };

        template<typename... Types, typename Type, typename... RightTypes>
        struct difference_type_st<tpf::type_list_t<Types...>, Type, RightTypes...>
        {
            using list = typename difference_type_st<tpf::type_list_t<Types...>, Type>::type;

            using type = std::conditional_t< sizeof...(RightTypes) == 0,
                list, typename difference_type_st<list, RightTypes...>::type>;
        };

        ////////////////////////////////////////////////////
        template<typename... Types>
        struct difference_type_st<tpf::type_list_t<Types...>, tpf::type_list_t<>>
        {
            using type = tpf::type_list_t<Types...>;
        };

        template<typename... Types, typename Type>
        struct difference_type_st<tpf::type_list_t<Types...>, tpf::type_list_t<Type>>
        {
            using type = typename difference_type_st<tpf::type_list_t<Types...>, Type>::type;
        };

        template<typename... Types, typename Type, typename... RightTypes>
        struct difference_type_st<tpf::type_list_t<Types...>, tpf::type_list_t<Type, RightTypes...>>
        {
           using type = typename difference_type_st<tpf::type_list_t<Types...>, Type, RightTypes...>::type;
        };

        template<typename LeftList, typename RightList>
        using difference_type_t = typename difference_type_st<LeftList, RightList>::type;

    } // end of namespace hide

    template<typename ArgType, typename TypeList>
    using remove_type_t = hide::remove_type_t<ArgType, TypeList>;

    template<typename LeftList, typename RightList>
    using difference_type_t = hide::difference_type_t<LeftList, RightList>;

    namespace hide
    {
        template<typename NewType, typename OldType, typename... Types>
        struct replace_type_st;

        template<typename NewType, typename OldType>
        struct replace_type_st<NewType, OldType>
        {
            using type = tpf::type_list_t<>;
        };

        template<typename NewType, typename OldType, typename Type>
        struct replace_type_st<NewType, OldType, Type>
        {
            using type = std::conditional_t< std::is_same_v<OldType, Type>,
                tpf::type_list_t<NewType>, tpf::type_list_t<Type>>;
        };

        template<typename NewType, typename OldType, typename Type, typename... RightTypes>
        struct replace_type_st<NewType, OldType, Type, RightTypes...>
        {
            using list = typename replace_type_st<NewType, OldType, Type>::type;

            using type = std::conditional_t< sizeof...(RightTypes) == 0,
                list, prepend_type_t<list, typename replace_type_st<NewType, OldType, RightTypes...>::type>>;
        };
        
        ///////////////////////////////////

        template<typename NewType, typename OldType>
        struct replace_type_st<NewType, OldType, tpf::type_list_t<>>
        {
            using type = tpf::type_list_t<>;
        };

        template<typename NewType, typename OldType, typename Type>
        struct replace_type_st<NewType, OldType, tpf::type_list_t<Type>>
        {
            using type = typename replace_type_st<NewType, OldType, Type>::type;
        };

        template<typename NewType, typename OldType, typename Type, typename... RightTypes>
        struct replace_type_st<NewType, OldType, tpf::type_list_t<Type, RightTypes...>>
        {
            using type = typename replace_type_st<NewType, OldType, Type, RightTypes...>::type;
        };

        template<typename NewType, typename OldType, typename TypeList>
        using replace_type_t = typename replace_type_st<NewType, OldType, TypeList>::type;

    } // end of namespace hide

    template<typename NewType, typename OldType, typename TypeList>
    using replace_type_t = hide::replace_type_t<NewType, OldType, TypeList>;

} // end of namespace tft

#endif // end of file