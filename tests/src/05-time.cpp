/*!
 * @file   05-time.cpp
 * @brief  Contains the unit tests for the time module.
 * @author lhm
 * @date   05/08/2019
 */

#define CATCH_CONFIG_MAIN 
#define WRITE_LOGS 0	/*!< Se to 1 to get output informations into 
							 the file "05-logs.txt" 				*/

#include <catch.hpp>
#include <stdbool.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>

#include <modelGenerator_interface.h>

using namespace ModelGeneratorAPI;

std::string VG_fileName("05-logs.txt");

TEST_CASE( "HHR DIANE is working correctly", "[time]" ) 
{
	if(WRITE_LOGS)
	{
		LOGS::setup(VG_fileName, 0);
	}

	SECTION("Input overflows") 
	{
		// (25h > 24h)
		/* mod 24h : 1h0min0s0ms
		 = 3 600 000 000 us
		 => HPF = 360 = b'0001 0110 1000' 
		 = b'1001 0110 1000' apres mise du bit de parite
		 => Hpf = HHR = 0
		 => l_res = HPF>>32 | Hpf>>16 | HHR = 1546188226560
		 */
		uint64_t l_input = 90000000000; // (25 * 3600 * 1000000;)
		uint64_t l_res   = TIME::getDIANEValue(l_input);
		
		if(WRITE_LOGS)
		{
			LOGS::write("Input overflows 1: (expected, received) (1546188226560, " 
						+ std::to_string(l_res) + ")", 0);
		}

		REQUIRE(l_res == 1546188226560);

		// (28h11min40s500ms > 24h)
		l_input = 101505000000; // ( 28*3600 + 11*60 + 40 + 0.5 )* 1000000 ;
		l_res   = TIME::getDIANEValue(l_input);

		/* mod 24h: 4h11min40s500ms
		 = 15 105 000 000 us
		 => HPF = 1 510 (15 105 000 000/ 10 000 000) = b'0000 0101 1110 0110'
		 => HPF = b'1000 0101 1110 0110' apres mise du bit de parité
		 => Hpf = 5000 ( 15 105 000 000% 10 000 000 ) = b'0001 0011 1000 1000'
		 => Hpf = b'1001 0011 1000 1000' apres mise du bit de parité
		 => HHR = 0

		 Finalement l_res = HPF>>32 | Hpf>>16 | HHR = 
		 b'1000 0101 1110 0110 1001 0011 1000 1000 0000 0000 0000 0000'
		 = 147 225 364 135 936‬
		*/

		if(WRITE_LOGS)
		{
			LOGS::write("Input overflows 2: (expected, received) (147225364135936, " 
						+ std::to_string(l_res) + ")", 0);
		}

		REQUIRE(l_res == 147225364135936);
	}

	SECTION("Valid inputs") 
	{
		// (3h17min4s300ms)
		/*
		 = 11 824 300 000us
		 => HPF = 1182 = b'0000 0100 1001 1110'
		 => Hpf = 4 300 000 / 1000 = 4300 = b'0001 0000 1100 1100'
		 		= b'1001 0000 1100 1100' apres mise du bit de parité
		 => HHR = 0
		 => l_res = HPF>>32 | Hpf>>16 | HHR = 5079080632320
		 */
		uint64_t l_input = 11824300000; // (3h17min4s300ms)
		uint64_t l_res   = TIME::getDIANEValue(l_input);

		if(WRITE_LOGS)
		{
			LOGS::write("Valid inputs 1: (expected, received) (5079080632320, " 
						+ std::to_string(l_res) + ")", 0);
		}

		REQUIRE(l_res == 5079080632320);

		// (1min6s100ms45us)
		/*
		 = 66 100 045us
		 => HPF = 6 = b'0000 0000 0000 0110'
		 => Hpf = 6 100 045 / 1000 = 6100 = b'0001 0111 1101 0100'
		 => HHR = 45 = b'0000 0000 0010 1101'
		 => l_res = HPF>>32 | Hpf>>16 | HHR = 26 169 573 421
		 */
		l_input = 66100045; // (1min6s100ms45us)
		l_res   = TIME::getDIANEValue(l_input);

		if(WRITE_LOGS)
		{
			LOGS::write("Valid inputs 2: (expected, received) (26169573421, " 
						+ std::to_string(l_res) + ")", 0);
		}

		REQUIRE(l_res == 26169573421);
	}

	if(WRITE_LOGS)
	{
		LOGS::close();
	}
}