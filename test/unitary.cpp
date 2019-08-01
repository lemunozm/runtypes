#include <runtypes/runtypes.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Unitary", "[types]")
{
    rt::Struct t1("my_type_1");
    t1.add_member("int", 5);

    REQUIRE(true);
    REQUIRE(false);
}
