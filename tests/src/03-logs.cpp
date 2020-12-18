/*!
 * @file   03-logs.cpp
 * @brief  Contains the unit tests for the Log module.
 * @author lhm
 * @date   05/08/2019
 */

#define CATCH_CONFIG_MAIN 

#include <catch.hpp>
#include <stdbool.h>
#include <cstdio>
#include <fstream>
#include <sstream>

#include <modelGenerator_interface.h>

using namespace ModelGeneratorAPI;

std::string get_lastLine(const std::string& p_fileName)
{
	std::string l_return("");
	std::ifstream fin;
    fin.open(p_fileName);
	if(fin.is_open()) 
	{
	    fin.seekg(-1, std::ios_base::end);

	    bool l_again = true;
	    while(l_again) 
	    {
	        char ch;
	        fin.get(ch);

	        if((int)fin.tellg() <= 1) {
	            fin.seekg(0);  
	            l_again = false; 
	        }
	        else if(ch == '\n') { 
	            l_again = false;
	        }
	        else { 
	            fin.seekg(-2, std::ios_base::cur); 
	        }
	    }
	     
	    getline(fin, l_return); 

	    fin.close();
	}

    return l_return;
}

TEST_CASE( "Logger setup works correctly", "[logs]" ) 
{
	SECTION("Incorrect fileName") 
	{
    	CHECK_THROWS( LOGS::setup("/incorrect/file/name", 0) );
	}

	SECTION("Empty fileName") 
	{
		std::string l_fileName = "";
    	CHECK_NOTHROW( LOGS::setup(l_fileName, 2) );
    	REQUIRE( (LOGS::isOpened() 	 == false) 	  );
    	LOGS::close();
	} 

	SECTION("Correct fileName") 
	{
		std::string l_fileName = "03-logs.txt";
		LOGS::setup(l_fileName, 2);
		REQUIRE( (LOGS::getLogFile() == l_fileName) );
		REQUIRE( (LOGS::isOpened() 	 == true) 		);

		// supprimer le fichier
		LOGS::close();
		std::remove(l_fileName.c_str());
	}
}

TEST_CASE( "Logger writing function works correctly", "[logs]" ) 
{
	std::string l_toWrite_1 = "Write when tracelevel is high enough";
	std::string l_toWrite_2 = "Do not write when specified tracelevel is insufficient";
	std::string l_fileName  = "03-logs.txt";

	SECTION("Write when tracelevel is high enough") 
	{
		LOGS::setup(l_fileName, 2);
    	LOGS::write(l_toWrite_1, 1);
    	LOGS::close();
    	std::string l_res = get_lastLine(l_fileName);
    	REQUIRE( l_toWrite_1.find(l_res) != std::string::npos );

    	std::remove(l_fileName.c_str());
	}

	SECTION("Do not write when specified tracelevel is insufficient") 
	{
		LOGS::setup(l_fileName, 1);
    	LOGS::write(l_toWrite_2, 2);
    	LOGS::close();
    	std::string l_res = get_lastLine(l_fileName);
    	REQUIRE( !l_toWrite_2.find(l_res) );

    	std::remove(l_fileName.c_str()); 
	}
}