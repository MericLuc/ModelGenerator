/*!
 * @file   02-version.cpp
 * @brief  Contains the unit tests for the version module.
 * @author lhm
 * @date   05/08/2019
 */

#define CATCH_CONFIG_MAIN 

#include <catch.hpp>
#include <modelGenerator_interface.h>

using namespace ModelGeneratorAPI;

TEST_CASE( "Version numbers are computed", "[version]" ) {
    REQUIRE( VERSION::VersionCommit() != "" );
    REQUIRE( VERSION::VersionBranch() != "" );
    REQUIRE( VERSION::Version() 	  != "" );
}