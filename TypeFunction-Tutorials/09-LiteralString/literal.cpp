#include "pch.h"
#include <cstdio>

auto stream = tpf::stream();
auto nl = stream.get_console_out();

// global literal string
const char* name1 = "Thomas Kim";

void test_literal_string()
{
    // local literal string, though the literal string "Thomas Kim"
    // is in this function, test_literal_string()
    // its memory is not on the stack
    // its memory is in the data section of the generated Assembler language
    const char* name2 = "Thomas Kim";

    printf("Address of name1 = %p\n", name1);
    printf("Address of name2 = %p\n", name2);

    /*
        name1 and name2 point to the same memory location.
        that is, name1 and name2 are the same objects.

        In C/C++ language, literal strings of the same content
        are merged together, and they are not created separately.

        If you ever have experience in Assembler language,
        you must already know of it.
    */

}

void deeper_understanding()
{
    // terminating zero is not included
    // but you cannot return name1
    const char name1[] = {'S','o','p','h','i','e'};
    
    // please note that name1 is not zero terminated string
    // it should crash...
    // GNU g++ complier is more C++ Standard - compliant or conformant
    std::cout << "name1 = [" << name1 << "]" << std::endl;
    stream << "name1 = [" << name1 << "]" << std::endl;

    // terminating zero is included
    // you cannot return name2
    // because name2 disappears when this block ends
    const char name2[] = "Sophie"; // {'S','o','p','h','i','e', '\0'};
    
    ((char*)name2)[0] = 'T';

      // since name2 is on the stack, it is not protected
    std::cout << "Name 2 = " << name2 << std::endl;

    // terminating zero is include
    // you can return the address of char*

    // data section is protected from modification
    const char* name3 = "Sophie";

    ((char*)name3)[0] = 'T';

    // it does not work, because data section is protected
    std::cout << "Name 3 = " << name3 << std::endl;

    printf("Address of name1 = %p\n", name1);
    printf("Address of name2 = %p\n", name2);
    printf("Address of name3 = %p\n", name3);

    /*
    name2 is a character array,
    name3 is const char* literal string
    name2 is allocated on the stack of this function
    that is, you cannot return the address of name2
    because when this block ends, name2 does not exist

    whereas name3 still exists after this block ends.
    it means we can still return the name3 as pointer
    because name3 is const char* and is initialized with literal string
    the literal string in C/C++ does not reside in stack
    but in data section.

    name2 is a stack variable
    name3's memory does not reside on the stack.
    */

    //////////////// Examine the bytesize of name1 and name2
    stream << "byte size of name1 = " << sizeof(name1) << nl;

    stream << "byte size of name2 = " << sizeof(name2) << nl;

    // it does not give the bytesize of the literal string    
    // sizeof(name3) returns the pointer size(const char*) of names
    // stream << "byte size of name3 = " << sizeof(name3) << nl;
    // terminating zero is included in the name3
    // but strlen() returns byte size not including zero terminator
    stream << "byte size of name3 = " << strlen(name3) << nl;
}

int main()
{
    // const char* my_name = "Thomas Kim";
    // printf("Address of my_name = %p\n", my_name);
    // test_literal_string();

    deeper_understanding();
}