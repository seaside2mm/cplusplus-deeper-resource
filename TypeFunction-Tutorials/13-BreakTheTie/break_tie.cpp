#include "pch.h"
#include "../include/tft_type_lib.h"

auto stream = tpf::stream();
auto nl = stream.get_console_out();

namespace tft
{
    template<typename T> // class template parameter T
    class ClassUnconnected
    {
        public:
            ClassUnconnected(const char* msg) // constructor parameter msg
            {                                  // between class template parameter T and constructor parameter msg
                                                // there is no connection of any kind

            }
    };

    // class template parameter (or argument) deduction guide
    template<typename T>
    ClassUnconnected(const T* msg)  // constructor
        ->
    ClassUnconnected<T>; // class template ... we are deducing the class template parameter T

    template<typename T> // class template parameter T
    class ClassTied
    {
        private:
            T m_value;

        public:
            ClassTied(const T* msg) // constructor parameter msg
            {                       // the type of msg is tied to T, the class template parameter
                                    // with this tie between constructor parameter and class template parameter T
                                    // C++ compiler can deduce the class template parameter
                                    // ClassTied<T>, this T can be deduced using constructor parameter

            }
    };

    // we can extract the type of T
    // using tft::element_type_t<ClassBreakTheTie<T>>
    template<typename T> // class template parameter T
    class ClassBreakTheTie // ClassBreakTheTile<char>
    {
        private:
            T m_value;

        public:

            // the custom deduction guide
            // modifies the type of m_value on-the-fly
            // this was impossible before C++17
            template<typename S> 
                ClassBreakTheTie(const S* msg): m_value{ msg }
            {                       
                
            }
    };

    // we defined class template parameter deduction guide for ClassBreakTheTie<S>
    template<typename S> // S -> char
    ClassBreakTheTie(const S* msg) -> // ClassBreakTheTie<char>::ClassBreakTheTie(const char* msg)
        ClassBreakTheTie<std::basic_string<S>>; // ClassBreakTheTie<std::basic_string<char>>

    // we have to break the tie between the constructor ClassBreakTheTie(const T* msg)
    // and the class ClassBreakTheTie<T>

} // end of namespace tft

void deduction_guide_basic()
{
    tft::ClassTied  t( "this is literal string");

    using t_type = decltype(t);
    stream <<"t's type: " << Tpf_GetTypeName(t_type) << nl;

    // it does not compile at all
    tft::ClassUnconnected u("This is literal string");

    using u_type = decltype(u);
    stream <<"u's type: " << Tpf_GetTypeName(u_type) << nl;

}

void deduction_guide_break_tie()
{
    tft::ClassBreakTheTie b("this is literal string");

    using b_type = decltype(b);
    using element_t = tft::element_type_t<b_type>;

    stream << "b\'s type : " << Tpf_GetTypeName(b_type) << nl;
    stream << "the element type T of b_type: " << Tpf_GetTypeName(element_t) << nl;

    ///////////////////////////////////
     tft::ClassBreakTheTie w(L"this is wide literal string");

    using w_type = decltype(w);
    using w_element_t = tft::element_type_t<w_type>;

    stream << "w\'s type : " << Tpf_GetTypeName(w_type) << nl;
    stream << "the element type T of w_type: " << Tpf_GetTypeName(w_element_t) << nl;

    /*
        Custom Deduction Guide enables us to EXTEND C++ Standard Library
        almost arbitrarily to meet our needs.

        In future sessions, I will show you how to extend C++ Standard Template Classes
        for our own use.
    */

}

int main()
{
    deduction_guide_break_tie();
}