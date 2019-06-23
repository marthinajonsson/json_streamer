
//
//// seven ways to detect and report the same error:
//BOOST_CHECK( add( 2,2 ) == 4 );        // #1 continues on error
//
//BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error
//
//if( add( 2,2 ) != 4 )
//BOOST_ERROR( "Ouch..." );            // #3 continues on error
//
//if( add( 2,2 ) != 4 )
//BOOST_FAIL( "Ouch..." );             // #4 throws on error
//
//if( add( 2,2 ) != 4 ) throw "Ouch..."; // #5 throws on error
//
//BOOST_CHECK_MESSAGE( add( 2,2 ) == 4,  // #6 continues on error
//"add(..) result: " << add( 2,2 ) );
//
//BOOST_CHECK_EQUAL( add( 2,2 ), 4 );	  // #7 continues on error


#define BOOST_TEST_MODULE write_tests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>
#include "write.h"

// entry point:
int main(int argc, char* argv[], char* envp[])
{
    return boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}