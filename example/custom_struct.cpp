#include <runtypes/runtypes.hpp>

#include <iostream>

struct A
{
    A() : c_(c)
    {
        c++;
        std::cout << "+ Build: " << c_ << ", address: " << reinterpret_cast<uintptr_t>(this) << std::endl;
    };

    A(int lc) : c_(lc)
    {
        std::cout << "+ Build by param: " << c_ << ", address: " << reinterpret_cast<uintptr_t>(this) << std::endl;
    };

    A(const A&) :c_(c)
    {
        c++;
        std::cout << "+ Copy: " << c_ << ", address: " << reinterpret_cast<uintptr_t>(this) << std::endl;
    };

    ~A()
    {
        std::cout << "- Destroy: " << c_ << ", address: " << reinterpret_cast<uintptr_t>(this) << std::endl;
    };

    static int c;
    int c_;
};

int A::c = 0;

int main()
{
    rt::Struct t1("my_type_1");
    t1.add_member("a", A());

    rt::Data d1(t1);
    d1["a"].set(A(9));

    rt::Data d2(d1);

    return 0;
}

