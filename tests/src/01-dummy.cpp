/*!
 * @file   01-dummy.cpp
 * @brief  Contains basic unit tests to better understand
 *		   the unit test lib.
 * @author lhm
 * @date   05/08/2019
 */

#define CATCH_CONFIG_MAIN 
#include <catch.hpp>

int Factorial( int number ) {
   //return number <= 1 ? number : Factorial( number - 1 ) * number;  // fail
   return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(0) == 1 );
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}