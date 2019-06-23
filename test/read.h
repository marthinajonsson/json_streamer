//
// Created by mjonsson on 6/23/19.
//

#ifndef JSON_STREAMER_READ_H
#define JSON_STREAMER_READ_H


#define BOOST_TEST_DYN_LINK
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <forward_list>
#include <boost/test/unit_test.hpp>
#include "../src/json_streamer.h"
#include <string>

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



struct R {

    std::string path;
    std::forward_list<std::string> _keys { "head", "id", "description", "value" };

    R() {
        BOOST_TEST_MESSAGE( "setup fixture" );
        boost::filesystem::path cwd(boost::filesystem::current_path());
        path = cwd.string() + "/cache.json";
    }
    ~R()         {
        BOOST_TEST_MESSAGE( "teardown fixture" );
        if (boost::filesystem::exists(path))
            boost::filesystem::remove(path);
    }
};

BOOST_FIXTURE_TEST_SUITE(read_tests, R)

    BOOST_AUTO_TEST_CASE(read_first_string) {

        struct Object {
            std::string head;
            std::string id;
            std::string desc;
            std::string value;
        } obj;

        obj.head = "parent";
        obj.id = "1";
        obj.desc = "testing";
        obj.value = "34";

        JsonStreamer<Object, std::string> streamer(path, _keys);
        streamer.write(obj);

        std::string test_string = "invalid";
        std::optional<Object> rsp = streamer.fetch(test_string);
        BOOST_CHECK_NE(rsp.has_value(), true);

        test_string = "parent";
        rsp = streamer.fetch(test_string);
        BOOST_CHECK_EQUAL(rsp.has_value(), true);
        BOOST_CHECK_EQUAL(rsp.value().desc, "testing");
    }

    BOOST_AUTO_TEST_CASE(read_second_int) {
        struct Object {
            int head;
            int id;
            int desc;
            int value;
        }obj;

        obj.head = 99;
        obj.id = 2;
        obj.desc = 322;
        obj.value = 99883;

        JsonStreamer<Object, int> streamer (path, _keys);
        streamer.write(obj);

        obj.head = 66;
        obj.id = 4;
        obj.desc = 3467;
        obj.value = 9;

        streamer.write(obj);

        int test_int = 33;
        std::optional<Object> rsp = streamer.fetch(test_int);
        BOOST_CHECK_NE(rsp.has_value(), true);

        test_int = 66;
        rsp = streamer.fetch(test_int);
        BOOST_CHECK_EQUAL(rsp.has_value(), true);
        BOOST_CHECK_EQUAL(rsp.value().desc, 3467);
    }


BOOST_AUTO_TEST_SUITE_END()


#endif //JSON_STREAMER_READ_H
