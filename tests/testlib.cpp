#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE( "Quick check", "[main]" ) {
    int x = 0;

REQUIRE( x == 0 );
}
