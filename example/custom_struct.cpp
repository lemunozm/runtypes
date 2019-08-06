#include <runtypes/runtypes.hpp>

#include <iostream>

#define INDENT_STR std::string(indent, ' ')

#define INDENT(VALUE) indent = VALUE;
#define LOG(VALUE) std::cout << INDENT_STR << "==== " VALUE " ====" << std::endl;

int indent;

struct A
{
    A(float value) : value_(value), c_(++c)
    {
        std::cout << INDENT_STR << "+ Build: " << c_ << "º, address: " << reinterpret_cast<uintptr_t>(this) << std::endl;
    };

    A(const A& other) : value_(other.value_),  c_(++c)
    {
        std::cout << INDENT_STR << "+ Copy: " << c_ << "º, address: " << reinterpret_cast<uintptr_t>(this) << std::endl;
    };

    ~A()
    {
        std::cout << INDENT_STR << "- Destroy: " << c_ << "º, address: " << reinterpret_cast<uintptr_t>(this) << std::endl;
    };

    static int c;
    float value_;
    int c_;
};

int A::c = 0;

int main()
{
    std::cout << "###### create and copy method ######" << std::endl;
    {
        INDENT(0);
        LOG("Struct definition");
        rt::Struct t1("my_type_1");
        t1.add_member("a", A(8.7f)); //copy way
        {
            INDENT(4);
            LOG("Data instantiation");
            rt::Data d1(t1);
            LOG("Set");
            d1["a"].set(A(9.5f));
            {
                INDENT(8);
                LOG("Data copy");
                rt::Data d2(d1);
                LOG("End data copy");
            }
            INDENT(4);
            LOG("End data instantation");
        }
        INDENT(0);
        LOG("End struct definition");
    }

    std::cout << std::endl;
    A::c = 0;

    std::cout << "###### emplace method ######" << std::endl;
    {
        INDENT(0);
        LOG("Struct definition");
        rt::Struct t1("my_type_1");
        t1.add_member<A>("a", 8.6f); //emplace way
        {
            INDENT(4);
            LOG("Data instantiation");
            rt::Data d1(t1);
            LOG("Set");
            d1["a"].set(A(9.5f));
            {
                INDENT(8);
                LOG("Data copy");
                rt::Data d2(d1);
                LOG("End data copy");
            }
            INDENT(4);
            LOG("End data instantation");
        }
        INDENT(0);
        LOG("End struct definition");
    }

    return 0;
}

