/*!
 * @file   04-model.cpp
 * @brief  Contains the unit tests for the Model module.
 * @author lhm
 * @date   05/08/2019
 */

#define CATCH_CONFIG_MAIN 
#define WRITE_LOGS 0	/*!< Set to 1 to get output informations into 
							 the file "04-logs.txt" 				*/

#include <catch.hpp>
#include <stdbool.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>

#include <modelGenerator_interface.h>

using namespace ModelGeneratorAPI;

std::string VG_fileName("04-logs.txt");

TEST_CASE( "Model creation is working correctly", "[model]" ) 
{
	if(WRITE_LOGS)
	{
		LOGS::setup(VG_fileName, 0);
	}

	SECTION("Incorrect fileName") 
	{
    	CHECK_THROWS( MODEL::create("/incorrect/file/name") );
	}

	SECTION("Empty fileName") 
	{
    	CHECK_THROWS( MODEL::create("") );
	}

	SECTION("Correct fileName") 
	{
		CHECK_NOTHROW( MODEL::create("./data/minimal_ok.xml") );
	}

	SECTION("Correct fileName but not properly formatted XML")
	{
		CHECK_THROWS(  MODEL::create("./data/wrong_xml.xml") );
	}

	SECTION("Correct fileName and XML format but param typo")
	{
		CHECK_THROWS(  MODEL::create("./data/typo.xml") );
	}

	SECTION("Correct fileName 2") 
	{
		CHECK_NOTHROW( MODEL::create("./data/minimal_ok.xml") );
	}
}

TEST_CASE( "Model running functions work correctly", "[model]" ) 
{
	if(WRITE_LOGS)
	{
		LOGS::setup(VG_fileName, 0);
	}

	CHECK_NOTHROW( MODEL::create("./data/running_ok.xml") );
	MODEL::log();

	std::vector<std::string> l_states;
	std::vector<std::string> l_states_w = 
		{ "ETAT_A", "ETAT_B", "ETAT_C",
		  "ETAT_B", "ETAT_D", "ETAT_D",
		  "ETAT_D", "ETAT_D", "ETAT_D",
		  "ETAT_B", "ETAT_E", "ETAT_B"
		};	

	std::vector< std::string > l_ipSrc;
	std::string l_ipSrc_w = "127.0.0.1";

	std::vector< std::string > l_ipDst;
	std::vector< std::string > l_ipDst_w = 
	{ "127.0.0.1"   , "10.101.80.10", "10.101.80.10",
	  "10.101.80.10", "10.101.80.10", "10.101.80.10",
	  "10.101.80.10", "10.101.80.10", "10.101.80.10",
	  "10.101.80.10", "127.0.0.1"   , "10.101.80.10"};

	std::vector< int > l_portSrc;
	std::vector< int > l_portSrc_w = 
	{ 1111, 2222, 2222, 
	  2222, 2222, 2222, 
	  2222, 2222, 2222, 
	  2222, 1111, 2222 };

	std::vector< int > l_portDst;
	std::vector< int > l_portDst_w = 
	{ 2222, 3333, 3333, 
	  3333, 3333, 3333, 
	  3333, 3333, 3333, 
	  3333, 2222, 3333 };

	for(unsigned i = 0; i < l_states_w.size(); i++)
	{
		MODEL::nextState();
        
		l_states.push_back(MODEL::currentStateString());

        auto l_messages = MODEL::getMessages();
        auto l_srcIp    = MODEL::getMessagesSrcIp();
        auto l_dstIp    = MODEL::getMessagesDstIp();
        auto l_srcPort  = MODEL::getMessagesSrcPort();
        auto l_dstPort  = MODEL::getMessagesDstPort();
        auto l_intface  = MODEL::getMessagesIntface();

        l_ipSrc.push_back(l_srcIp[0]);
        l_ipDst.push_back(l_dstIp[0]);
        l_portSrc.push_back(l_srcPort[0]);
        l_portDst.push_back(l_dstPort[0]);

        ModelGeneratorAPI::MODEL::runOperations();
        ModelGeneratorAPI::MODEL::runTransitions();
	}

	SECTION("The states transitions are working properly") 
	{
		if(WRITE_LOGS)
		{
			LOGS::write("The states transitions are working properly", 0);
		}

		for(unsigned i = 0; i < l_states.size(); i++)
		{
			REQUIRE( l_states[i] == l_states_w[i]);
			if(WRITE_LOGS)
			{
				LOGS::write("(expected, received) (" +
							l_states_w[i] + ", " 
							+ l_states[i] + ")", 0);
			}
		}
	}

	SECTION("The source IPs are correct")
	{
		if(WRITE_LOGS)
		{
			LOGS::write("The source IPs are correct", 0);
		}

		for(unsigned i = 0; i < l_ipSrc.size(); i++)
		{
			REQUIRE( l_ipSrc[i] == l_ipSrc_w);

			if(WRITE_LOGS)
			{
				LOGS::write("(expected, received) (" +
								l_ipSrc_w + ", " 
								+ l_ipSrc[i] + ")", 0);
			}
		}
	}

	SECTION("The destination IPs are correct")
	{
		if(WRITE_LOGS)
		{
			LOGS::write("The destination IPs are correct", 0);
		}

		for(unsigned i = 0; i < l_ipDst.size(); i++)
		{
			REQUIRE( l_ipDst[i] == l_ipDst_w[i]);

			if(WRITE_LOGS)
			{
				LOGS::write("(expected, received) (" +
								l_ipDst_w[i] + ", " 
								+ l_ipDst[i] + ")", 0);
			}
		}
	}

	SECTION("The source ports are correct")
	{
		if(WRITE_LOGS)
		{
			LOGS::write("The source ports are correct", 0);
		}

		for(unsigned i = 0; i < l_portSrc.size(); i++)
		{
			REQUIRE( l_portSrc[i] == l_portSrc_w[i]);

			if(WRITE_LOGS)
			{
				LOGS::write("(expected, received) (" +
								std::to_string(l_portSrc_w[i]) + ", " 
								+ std::to_string(l_portSrc[i]) + ")", 0);
			}
		}
	}

	SECTION("The destination ports are correct")
	{
		if(WRITE_LOGS)
		{
			LOGS::write("The source ports are correct", 0);
		}

		for(unsigned i = 0; i < l_portDst.size(); i++)
		{
			REQUIRE( l_portDst[i] == l_portDst_w[i]);

			if(WRITE_LOGS)
			{
				LOGS::write("(expected, received) (" +
								std::to_string(l_portDst_w[i]) + ", " 
								+ std::to_string(l_portDst[i]) + ")", 0);
			}
		}
	}

	if(WRITE_LOGS)
	{
		LOGS::close();
	}
}

TEST_CASE( "SIMPLE MODE: Model messages are correctly built", "[model]" ) 
{
	if(WRITE_LOGS)
	{
		LOGS::setup(VG_fileName, 0);
	}

	CHECK_NOTHROW( MODEL::create("./data/messages_ok.xml") );
	MODEL::log();

	/*!
	 * 			-- Message associated to STATE A --
	 * 	// HEADER //
	 * |   NAME      |   VALUE   |   SIZE   |        BIN       |
	 *    FIELD_1         16          8           0001 0000
	 *    FIELD_2         90          8		      0101 1010
	 *    FIELD_TIME      ?           32              ?
	 *    FIELD_SIZE     19bytes      8			  0001 0011
	 *    FIELD_ID       52 275		  16	  1100 1100 0011 0011
	 * // MESG_1 //
	 * 10 octets à b'0000 0000'
	 */
	unsigned char index_start_time = 2;
	unsigned char index_end_time   = 5;

	std::vector< unsigned char > l_mesgA_w = { 	16, 			// FIELD_1
												90, 			// FIELD_2
												0, 0, 0, 0, 	// FIELD_TIME
												19, 			// FIELD_SIZE
												204, 51, 		// FIELD_ID
												0, 0, 0, 0, 0, 
												0, 0, 0, 0, 0   // MESG_1
											};
	MODEL::nextState();
    auto l_msg 		= MODEL::getMessages();

	SECTION("Message of State A is correct")
	{
		LOGS::write(MODEL::currentStateString(), 0);
    	LOGS::write("Message size: "  + std::to_string(l_msg[0].size()),  0);
    	LOGS::write("Expected size: " + std::to_string(l_mesgA_w.size()), 0);
    	REQUIRE(l_msg[0].size() == l_mesgA_w.size());

		if(WRITE_LOGS)
		{
			LOGS::write("Message of State A is correct", 0);
		}

		for(unsigned i = 0; i < l_msg[0].size(); i++)
		{
			if( (i < index_start_time) || (i > index_end_time) )
			{
				REQUIRE( l_msg[0][i] == l_mesgA_w[i] );

				if(WRITE_LOGS)
				{
					LOGS::write("octet " + std::to_string(i) + ": (expected, received) ("
									+ std::to_string(l_mesgA_w[i]) + ", " 
									+ std::to_string(l_msg[0][i])  + ")", 0);
				}
			}
		}
	}

	/*!
	 * 			-- Message associated to STATE B --
	 * 	// HEADER //
	 * |   NAME      |   VALUE   |   SIZE   |        BIN       |
	 *    FIELD_1         1           1           	  1
	 *    FIELD_2         9           7	           000 1001
	 *    FIELD_TIME      ?           32              ?
	 *    FIELD_SIZE     50      	  8			  0011 0010 (mots de 16 bits)
	 * // MESG_2 //
	 * 100 octets à aléatoires
	 */
	std::vector< unsigned char > l_mesgB_w(106, 0);
	l_mesgB_w[0] = 137;  // FIELD_1 && FIELD_2
	l_mesgB_w[5] = 50;   // FIELD_SIZE

    ModelGeneratorAPI::MODEL::runOperations();
    ModelGeneratorAPI::MODEL::runTransitions();
    MODEL::nextState();
    l_msg 		= MODEL::getMessages();

	SECTION("Message of State B is correct")
	{
		LOGS::write(MODEL::currentStateString(), 0);
    	LOGS::write("Message size: "  + std::to_string(l_msg[0].size()),  0);
    	LOGS::write("Expected size: " + std::to_string(l_mesgB_w.size()), 0);
    	REQUIRE(l_msg[0].size() == l_mesgB_w.size());

		if(WRITE_LOGS)
		{
			LOGS::write("Message of State B is correct", 0);
		}

		REQUIRE( l_msg[0][0] == l_mesgB_w[0] );
		REQUIRE( l_msg[0][5] == l_mesgB_w[5] );

		if(WRITE_LOGS)
		{
			LOGS::write("octet 0: (expected, received) ("
						+ std::to_string(l_mesgB_w[0]) + ", " 
						+ std::to_string(l_msg[0][0])  + ")", 0);

			LOGS::write("octet 5: (expected, received) ("
						+ std::to_string(l_mesgB_w[5]) + ", " 
						+ std::to_string(l_msg[0][5])  + ")", 0);
		}
	}

	/*!
	 * 			-- Message associated to STATE C --
	 * 	// HEADER //
	 * |   NAME      |   VALUE   |   SIZE   |        BIN       |
	 *    FIELD_1          90         8           0101 1010
	 *    FIELD_2         195         8	          1100 0011
	 *	  FIELD_3		  519        10           1000 0001 11
	 *    FIELD_4           2        16           0000 0000 0000 0010
	 *    FIELD_SIZE     18(10+8)     8			  00001 0011 (mots de 8 bits)
	 *    FIELD_5	        5         3                   101
	 * // MESG_1 //
	 * 10 octets à b'0000 0000'
	 */
	std::vector< unsigned char > l_mesgC_w = { 	90, 			// FIELD_1
												195, 			// FIELD_2
												129, 192, 		// FIELD_3 (1000 0001 11 '00 0000')
												0, 2,			// FIELD_4 (0000 0000 0000 0010)
												18, 			// FIELD_SIZE
												160, 			// FIELD_5 (101 '0 0000')
												0, 0, 0, 0, 0, 
												0, 0, 0, 0, 0   // MESG_1
											};

    ModelGeneratorAPI::MODEL::runOperations();
    ModelGeneratorAPI::MODEL::runTransitions();
    MODEL::nextState();
    l_msg 		= MODEL::getMessages();

	SECTION("Message of State C is correct")
	{
		LOGS::write(MODEL::currentStateString(), 0);
    	LOGS::write("Message size: "  + std::to_string(l_msg[0].size()),  0);
    	LOGS::write("Expected size: " + std::to_string(l_mesgC_w.size()), 0);
    	REQUIRE(l_msg[0].size() == l_mesgC_w.size());

		if(WRITE_LOGS)
		{
			LOGS::write("Message of State C is correct", 0);
		}

		for(unsigned i = 0; i < l_msg[0].size(); i++)
		{
			REQUIRE( l_msg[0][i] == l_mesgC_w[i] );

			if(WRITE_LOGS)
			{
				LOGS::write("octet " + std::to_string(i) + ": (expected, received) ("
							+ std::to_string(l_mesgC_w[i]) + ", " 
							+ std::to_string(l_msg[0][i])  + ")", 0);
			}
		}
	}

	if(WRITE_LOGS)
	{
		LOGS::close();
	}
}

TEST_CASE( "MULTIPLEXED MODE: Model messages are correctly built", "[model]" ) 
{
	if(WRITE_LOGS)
	{
		LOGS::setup(VG_fileName, 0);
	}

	CHECK_NOTHROW( MODEL::create("./data/messages_ok_multiplexed.xml") );
	MODEL::log();

	/*!
	 * 			-- Messages associated to STATE A --
	 *
	 *=======================MESSAGE 1========================
	 * 	// HEADER_1//
	 * |   NAME      |   VALUE   |   SIZE   |        BIN       |
	 *    FIELD_1         16          8           0001 0000
	 *    FIELD_2         90          8		      0101 1010
	 *    FIELD_TIME      ?           32              ?
	 *    FIELD_SIZE     19bytes      8			  0001 0011
	 *    FIELD_ID       52 275		  16	  1100 1100 0011 0011
	 * // MESG_1 //
	 * 10 octets à b'0000 0000'
	 *
	 *
	 *=======================MESSAGE 2========================
	 * 	// HEADER_2 //
	 * |   NAME      |   VALUE   |   SIZE   |        BIN       |
	 *    FIELD_1         1           1           	  1
	 *    FIELD_2         9           7	           000 1001
	 *    FIELD_TIME      ?           32              ?
	 *    FIELD_SIZE     50      	  8			  0011 0010 (mots de 16 bits)
	 * // MESG_2 //
	 * 100 octets à aléatoires
	 */
	std::vector< unsigned char > l_mesgB_w(106, 0);
	l_mesgB_w[0] = 137;  // FIELD_1 && FIELD_2
	l_mesgB_w[5] = 50;   // FIELD_SIZE
	unsigned char index_start_time = 2;
	unsigned char index_end_time   = 5;

	std::vector< unsigned char > l_mesgA_w = { 	16, 			// FIELD_1
												90, 			// FIELD_2
												0, 0, 0, 0, 	// FIELD_TIME
												19, 			// FIELD_SIZE
												204, 51, 		// FIELD_ID
												0, 0, 0, 0, 0, 
												0, 0, 0, 0, 0   // MESG_1
											};
	MODEL::nextState();
    auto l_msg 		= MODEL::getMessages();

    SECTION("Number of messages is correct")
    {
    	if(WRITE_LOGS)
		{
			LOGS::write("The number of multiplexed messages is correct", 0);
    		LOGS::write("Number of messages: "  + std::to_string(l_msg.size()),  0);
    		LOGS::write("Expected number   : 2", 0);
    	}

    	REQUIRE(l_msg.size() == 2);
    }

	SECTION("Message 1 is correct")
	{
		if(WRITE_LOGS)
		{	LOGS::write("The size of multiplexed message 1 is correct", 0);
	    	LOGS::write("Message size : "  + std::to_string(l_msg[0].size()),  0);
	    	LOGS::write("Expected size: "  + std::to_string(l_mesgA_w.size()), 0);
	    }
    	REQUIRE(l_msg[0].size() == l_mesgA_w.size());

		if(WRITE_LOGS)
		{
			LOGS::write("Multiplexed message 1 is correct", 0);
		}

		for(unsigned i = 0; i < l_msg[0].size(); i++)
		{
			if( (i < index_start_time) || (i > index_end_time) )
			{
				REQUIRE( l_msg[0][i] == l_mesgA_w[i] );

				if(WRITE_LOGS)
				{
					LOGS::write("octet " + std::to_string(i) + ": (expected, received) ("
									+ std::to_string(l_mesgA_w[i]) + ", " 
									+ std::to_string(l_msg[0][i])  + ")", 0);
				}
			}
		}
	}

	SECTION("Message 2 is correct")
	{
		if(WRITE_LOGS)
		{	LOGS::write("The size of multiplexed message 2 is correct", 0);
	    	LOGS::write("Message size : "  + std::to_string(l_msg[1].size()),  0);
	    	LOGS::write("Expected size: "  + std::to_string(l_mesgB_w.size()), 0);
	    }
    	REQUIRE(l_msg[1].size() == l_mesgB_w.size());

		if(WRITE_LOGS)
		{
			LOGS::write("Multiplexed message 2 is correct", 0);
		}

		REQUIRE( l_msg[1][0] == l_mesgB_w[0] );
		REQUIRE( l_msg[1][5] == l_mesgB_w[5] );

		if(WRITE_LOGS)
		{
			LOGS::write("octet 0: (expected, received) ("
						+ std::to_string(l_mesgB_w[0]) + ", " 
						+ std::to_string(l_msg[1][0])  + ")", 0);

			LOGS::write("octet 5: (expected, received) ("
						+ std::to_string(l_mesgB_w[5]) + ", " 
						+ std::to_string(l_msg[1][5])  + ")", 0);
		}
	}

	if(WRITE_LOGS)
	{
		LOGS::close();
	}
}