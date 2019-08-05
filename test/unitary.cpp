#include <runtypes/runtypes.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <array>

template <typename T>
void test_data(rt::WritableDataRef&& d, const T& value, const T& set_value)
{
    WHEN("data from '" + d.type().name() + "' is accessed")
    {
        THEN("data is read")
        {
            //first way
            REQUIRE(d.get<T>() == value);

            //second way
            T get_value;
            d.get(get_value);
            REQUIRE(get_value == value);
        }

        THEN("data is wrote")
        {
            d.set<T>(set_value);
            REQUIRE(d.get<T>() == set_value);
        }
    }
}

template <typename T>
void test_c_type(const std::string& name_type, const T& value, const T& set_value)
{
    WHEN("'" + name_type + "' is declared")
    {
        rt::CType<T> t(value);

        THEN("c type '" + std::string{typeid(T).name()} + "' is created")
        {
            REQUIRE(t.kind() == rt::Kind::CType);
            REQUIRE(t.name() == typeid(T).name());
            REQUIRE(t.memory_size() == sizeof(T));
            REQUIRE(t.hash_code() == typeid(T).hash_code());
            REQUIRE(t.base_instance() == value); //This checks works only with comparable types
        }

        WHEN("data from '" + t.name() + "' is created")
        {
            rt::Data d(t);

            THEN("data and type are associated")
            {
                REQUIRE(d.type().name() == t.name());
            }

            test_data(std::move(d), value, set_value);
        }
    }
}

template <typename T>
void test_add_c_member(rt::Struct& s, const std::string& name, const T& value)
{
    size_t struct_memory_size = s.memory_size();
    s.add_member(name, value);

    THEN("the c type member '" + name + "' is added")
    {
        REQUIRE(s[name].kind() == rt::Kind::CType);
        REQUIRE(s[name].name() == typeid(T).name());
        REQUIRE(s[name].memory_size() == sizeof(T));
        REQUIRE(static_cast<const rt::CType<T>&>(s[name]).hash_code() == typeid(T).hash_code());
        REQUIRE(static_cast<const rt::CType<T>&>(s[name]).base_instance() == value);
        REQUIRE(s.member(name) != nullptr);
        REQUIRE(s.member(name)->offset() == struct_memory_size);
        REQUIRE(&s.member(name)->type() == &s[name]);
    }
}

template <typename T, typename... Args>
void test_emplace_c_member(rt::Struct& s, const std::string& name, Args... args)
{
    size_t struct_memory_size = s.memory_size();
    s.emplace_member<T>(name, std::forward<Args>(args)...);

    THEN("the c type member '" + name + "' is emplaced")
    {
        REQUIRE(s[name].kind() == rt::Kind::CType);
        REQUIRE(s[name].name() == typeid(T).name());
        REQUIRE(s[name].memory_size() == sizeof(T));
        REQUIRE(static_cast<const rt::CType<T>&>(s[name]).hash_code() == typeid(T).hash_code());
        REQUIRE(static_cast<const rt::CType<T>&>(s[name]).base_instance() == T(std::forward<Args>(args)...));
        REQUIRE(s.member(name) != nullptr);
        REQUIRE(s.member(name)->offset() == struct_memory_size);
        REQUIRE(&s.member(name)->type() == &s[name]);
    }
}

void test_add_struct_member(rt::Struct& s, const std::string& name, rt::Struct& inner)
{
    size_t struct_memory_size = s.memory_size();
    s.add_member(name, inner);

    THEN("the struct type member '" + name + "' is added")
    {
        REQUIRE(s[name].kind() == rt::Kind::Struct);
        REQUIRE(s[name].name() == inner.name());
        REQUIRE(s[name].memory_size() == inner.memory_size());
        REQUIRE(s.member(name) != nullptr);
        REQUIRE(s.member(name)->offset() == struct_memory_size);
        REQUIRE(&s.member(name)->type() == &s[name]);
    }
}

SCENARIO("unitary test")
{
    GIVEN("c types list")
    {
        test_c_type("bool", true, false);
        test_c_type("char", 'A', 'B');
        test_c_type("short", (short) 5, (short) 7);
        test_c_type("int", 5, 8);
        test_c_type("long int", 9L, 6L);
        test_c_type("long long int", 4LL, 7LL);
        test_c_type("float", 3.0f, 8.0f);
        test_c_type("double", 8.0, 6.0);

        test_c_type("unsigned short", (unsigned short) 5,(unsigned short) 9);
        test_c_type("unsigned int", 5U, 8U);
        test_c_type("unsigned long int", 9UL, 1UL);
        test_c_type("unsigned long long int", 4ULL, 6ULL);

        test_c_type("int8_t", (int8_t) 1, (int8_t) 9);
        test_c_type("int16_t", (int16_t) 2, (int16_t) 10);
        test_c_type("int32_t", (int32_t) 3, (int32_t) 11);
        test_c_type("int64_t", (int64_t) 4, (int64_t) 12);
        test_c_type("uint8_t", (uint8_t) 5, (uint8_t) 13);
        test_c_type("uint16_t", (uint16_t) 6, (uint16_t) 14);
        test_c_type("uint32_t", (uint32_t) 7, (uint32_t) 15);
        test_c_type("uint64_t", (uint64_t) 8, (uint64_t) 16);

        int a; int b;
        test_c_type("int*", &a, &b);
        test_c_type("const int*", (const int*) &a, (const int*) &b);

        test_c_type("string", std::string{"hello"}, std::string{"bye"});
        test_c_type("array", std::array<int, 4>{1, 2, 4, 8}, std::array<int, 4>{19, 7, 2, 3});
        test_c_type("vector", std::vector<int>{1, 2, 4, 8}, std::vector<int>{4, 5});
        test_c_type("map", std::map<std::string, int>{{"a", 3}, {"b", 8}, {"c", 7}}, std::map<std::string, int>{{"a", 3}});
    }

    GIVEN("a structure")
    {
        rt::Struct basic("basic");
        WHEN("is declared")
        {
            REQUIRE(basic.name() == "basic");
        }

        WHEN("add c members")
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
            test_add_c_member(basic, "array", std::array<int, 4>{1, 2, 4, 8});
            test_add_c_member(basic, "vector", std::vector<int>{1, 2, 4, 8});
            test_add_c_member(basic, "map", std::map<std::string, int>{{"a", 3}, {"b", 8}, {"c", 7}});

            struct A
            {
                A() {}
                A(const A&) {} //only requirement is to be copy constructible
                bool operator == (const A&) const { return true; } // used in the test only, no API mandatory
            };
            test_add_c_member(basic, "custom struct", A());

            test_emplace_c_member<int>(basic, "emplace char", 'A');
            test_emplace_c_member<int>(basic, "emplace int", 8);
            test_emplace_c_member<std::string>(basic, "emplace string", "hello");
            test_emplace_c_member<std::vector<int>>(basic, "emplace vector", 5, 1);

            struct B
            {
                B(int a, int b, int c) : a_(a), b_(b), c_(c) {}
                B(const B&) {} //only requirement is to be copy constructible
                bool operator == (const B&) const { return true; } // required only for the test, no API mandatory
                int a_; int b_; int c_;
            };

            test_emplace_c_member<B>(basic, "emplace custom struct", 3, 6, 9);

            WHEN("data is created")
            {
                rt::Data d1(basic);
                rt::Data d2(basic); //to check data overlap

                WHEN("d1 is accessed")
                {
                    test_data(d1["int"], 5, 2);
                    test_data(d1["float"], 3.0f, 9.5f);
                    test_data(d1["string"], std::string{"hello"}, std::string{"bye"});

                    WHEN("d2 is accessed")
                    {
                        test_data(d2["int"], 5, 3);
                        test_data(d2["float"], 3.0f, 9.5f);
                        test_data(d2["string"], std::string{"hello"}, std::string{"bye"});

                        WHEN("d3 is accessed")
                        {
                            //to check data overlap
                            rt::Data d3(basic);
                            test_data(d3["int"], 5, 3);
                            test_data(d3["float"], 3.0f, 9.5f);
                            test_data(d3["string"], std::string{"hello"}, std::string{"bye"});

                        }
                    }
                }

                WHEN("data is copied and accessed")
                {
                    rt::Data d4(basic);
                    d4["int"].set(2);
                    d4["float"].set(4.2f);
                    d4["string"].set(std::string{"was modified!"});

                    rt::Data d5(d4); //copy here

                    test_data(d5["int"], 2, 9);
                    test_data(d5["float"], 4.2f, 1.3f);
                    test_data(d5["string"], std::string{"was modified!"}, std::string{"was copied!"});

                    THEN("sources not change")
                    {
                        test_data(d4["int"], 2, 1);
                        test_data(d4["float"], 4.2f, 2.1f);
                        test_data(d4["string"], std::string{"was modified!"}, std::string{"not was changed"});
                    }
                }
            }
        }
    }

    GIVEN("some nested structures")
    {
        rt::Struct bottom1("bottom1");
        rt::Struct bottom2("bottom2");
        rt::Struct middle("middle");
        rt::Struct top("top");

        WHEN("add members")
        {
            test_add_c_member(bottom1, "int", 7);
            test_add_c_member(bottom1, "vector", std::vector<int>{1, 2, 4, 8});

            test_add_c_member(bottom2, "float", 4.0f);

            test_add_c_member(middle, "int", 3);
            test_add_struct_member(middle, "inner1", bottom1);
            test_add_struct_member(middle, "inner2", bottom2);

            test_add_struct_member(top, "inner1", middle);
            test_add_struct_member(top, "inner2", bottom1);

            WHEN("data is created")
            {
                rt::Data m(middle);
                rt::Data t1(top);
                rt::Data t2(top); //TO check data overlap

                WHEN("middle is accessed")
                {
                    test_data(m["inner1"]["int"], 7, 9);
                }
                WHEN("top1 is accessed")
                {
                    test_data(t1["inner1"]["inner1"]["int"], 7, 2);
                }
                WHEN("top2 is accessed")
                {
                    test_data(t2["inner2"]["int"], 7 ,4);
                }
            }
        }
    }
}
