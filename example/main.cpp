#include <runtypes/Type.hpp>
#include <runtypes/Data.hpp>

#include <iostream>

int main()
{
    // API
    rt::Struct t1("my_type_1");
    t1.add_member("int", 5);
    t1.add_member("vec", std::vector<int>{4, 5, 3});
    t1.add_member("map", std::map<std::string, int>{{"a", 3}, {"b", 8}, {"c", 7}});

    rt::Struct t2("my_type_2");
    t2.add_member("float", 3.2f);
    t2.add_member("inner", t1);

    rt::Data d1(t1);
    rt::Data d2(t2);

    // CHECKS
    std::cout << d1["int"].type().name() << std::endl;
    std::cout << d1["vec"].type().name() << std::endl;
    std::cout << d1["map"].type().name() << std::endl;
    std::cout << d2["float"].type().name() << std::endl;
    std::cout << d2["inner"].type().name() << std::endl;
    std::cout << d2["inner"]["int"].type().name() << std::endl;
    std::cout << d2["inner"]["vec"].type().name() << std::endl;
    std::cout << d2["inner"]["map"].type().name() << std::endl;

    d1["int"].set(8);
    d2["inner"]["vec"].set(std::vector<int>{2, 4, 6, 8, 10});
    std::cout << d1["int"].get<int>() << std::endl;
    std::cout << d2["inner"]["vec"].get<std::vector<int>>()[2] << std::endl;

    return 0;
}

