#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <mirr/mirr.h>

TEST_CASE("Check register_type", "[register_type]")
{
    mirr::register_type<float>("float");

    REQUIRE(mirr::resolve("float").valid());
    REQUIRE(mirr::valid("float"));
}

TEST_CASE("Check register_type name", "[register_type name]")
{
    mirr::register_type<int32_t>("int32");

    REQUIRE(mirr::resolve("int32").name() == "int32");
}

TEST_CASE("Check type flags", "[type flags]")
{
    struct vector
    {
        float x;
        float y;
        float z;
    };

    mirr::register_type<vector>("vector");

    auto type = mirr::resolve<vector>();

    REQUIRE_FALSE(type.is_pointer());
    REQUIRE_FALSE(type.is_associative_container());
    REQUIRE_FALSE(type.is_integral());
    REQUIRE(type.is_class());
    REQUIRE_FALSE(type.is_sequence_container());
    REQUIRE_FALSE(type.is_pointer_like());
    REQUIRE_FALSE(type.is_template_specialization());
    REQUIRE_FALSE(type.is_abstract());
    REQUIRE_FALSE(type.is_array());
    REQUIRE_FALSE(type.is_enum());

}