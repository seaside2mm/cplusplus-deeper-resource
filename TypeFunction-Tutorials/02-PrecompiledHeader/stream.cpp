#include "../include/pch.h"

int main()
{
    auto stream = tpf::stream();
    auto nl = stream.get_console_out();

    stream << "Hello, Type Functions!" << nl;

    using vector_t = std::vector<int>;

    vector_t ints { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    stream << ints << nl;

    using name_t = std::string;

    using names_t = std::vector<name_t>;

    names_t names { "Thomas Kim", "Steven Park", "Sophie Turner", "Demi Moore"};

    stream << names << nl;


    using age_t = int;
    using weight_t = double;
    using person_t = std::tuple<name_t, age_t, weight_t>;

    using people_t = std::vector<person_t>;

    people_t students {  {"Thomas Kim", 19, 56.0 }, {"Steve Lee", 18, 80.0 }, {"James Dean", 50, 70.0 },  };

    stream << "My students: \n" << students << nl;

    int array[] = {1, 2, 3, 4, 5};

    stream << "int array: \n" << array << nl;

    using item_t = std::variant<name_t, age_t, weight_t>;
    using bag_t = std::deque<item_t>;

    bag_t mybag { "The Good Man", 20, 50.6, "The Future of C++", "The Best C++ Tutor", 60.7, 30 };

    stream <<"In my bag, there items as below: \n"
        << mybag << nl;

    int darray [][3] = { {3, 4, 6}, {4, 4, 6}, {5, 4, 6}, {6, 4, 6} };

    stream << "darray \n: " << darray << nl;


    auto && ra = 5.6;

    stream << "Type name of ra = " << Tpf_GetTypeCategory(ra) << nl;
    

}