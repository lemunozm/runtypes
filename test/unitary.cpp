#include <runtypes/runtypes.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <array>

template <typename T>
void test_add_c_member(rt::Struct& s, const std::string& name, T&& value)
{
    size_t struct_memory_size = s.memory_size();

    s.add_member(name, value);

    THEN("added '" + name + "' member")
    {
        REQUIRE(s[name].kind() == rt::Kind::CType);
        REQUIRE(s[name].name() == typeid(T).name());
        REQUIRE(s[name].memory_size() == sizeof(T));
        REQUIRE(s.member(name) != nullptr);
        REQUIRE(s.member(name)->offset() == struct_memory_size);
        REQUIRE(&s.member(name)->type() == &s[name]);
    }
}

SCENARIO("type structure definition", "[runtypes]")
{
    GIVEN("simple struct")
    {
        rt::Struct basic("my_type_1");

        WHEN("initializing c members")
        {
            test_add_c_member(basic, "bool", true);
            test_add_c_member(basic, "char", 'A');
            test_add_c_member(basic, "short", (short)5);
            test_add_c_member(basic, "int", 5);
            test_add_c_member(basic, "long int", 9L);
            test_add_c_member(basic, "long long int", 4LL);
            test_add_c_member(basic, "float", 3.0f);
            test_add_c_member(basic, "double", 8.0);

            test_add_c_member(basic, "unsigned short", (unsigned short)5);
            test_add_c_member(basic, "unsigned int", 5U);
            test_add_c_member(basic, "unsigned long int", 9UL);
            test_add_c_member(basic, "unsigned long long int", 4ULL);

            test_add_c_member(basic, "int8_t", (int8_t)1);
            test_add_c_member(basic, "int16_t", (int16_t)2);
            test_add_c_member(basic, "int32_t", (int32_t)3);
            test_add_c_member(basic, "int64_t", (int64_t)4);
            test_add_c_member(basic, "uint8_t", (uint8_t)5);
            test_add_c_member(basic, "uint16_t", (uint16_t)6);
            test_add_c_member(basic, "uint32_t", (uint32_t)7);
            test_add_c_member(basic, "uint64_t", (uint64_t)8);

            int a;
            test_add_c_member(basic, "int*", (int*)&a);
            test_add_c_member(basic, "const int*", (const int*)&a);

            test_add_c_member(basic, "string", std::string{"hello"});
            test_add_c_member(basic, "long string", std::string{"hello, I am the longest string ever :D"});
            test_add_c_member(basic, "array", std::array<int, 4>{1, 2, 4, 8});
            test_add_c_member(basic, "vector", std::vector<int>{1, 2, 4, 8});
            test_add_c_member(basic, "map", std::map<std::string, int>{{"a", 3}, {"b", 8}, {"c", 7}});
        }
    }
}
