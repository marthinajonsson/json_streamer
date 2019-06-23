//
// Created by mjonsson on 6/22/19.
//

#ifndef JSON_STREAMER_WRITE_H
#define JSON_STREAMER_WRITE_H

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


struct W {

    std::string path;
    std::forward_list<std::string> _keys { "head", "id", "description", "value" };

    W() {
        BOOST_TEST_MESSAGE( "setup fixture" );
        boost::filesystem::path cwd(boost::filesystem::current_path());
        path = cwd.string() + "/cache.json";
    }
    ~W()         {
        BOOST_TEST_MESSAGE( "teardown fixture" );
        if (boost::filesystem::exists(path))
            boost::filesystem::remove(path);
    }
};

BOOST_FIXTURE_TEST_SUITE(write_tests, W)

    BOOST_AUTO_TEST_CASE(write_string) {
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

        boost::property_tree::ptree pt;
        boost::property_tree::read_json(path, pt);
        pt = pt.get_child("");

        BOOST_FOREACH(boost::property_tree::ptree::value_type &node, pt) {
                    auto head = node.second.get<std::string>("head");
                    BOOST_CHECK_EQUAL(head, obj.head);
                    auto data = node.second.get_child("data");
                    auto desc = data.get<std::string>("description");
                    BOOST_CHECK_EQUAL(desc, obj.desc);
        }

    }

    BOOST_AUTO_TEST_CASE(write_int) {
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

        boost::property_tree::ptree pt;
        boost::property_tree::read_json(path, pt);
        pt = pt.get_child("");

        BOOST_FOREACH(boost::property_tree::ptree::value_type& node , pt) {
                    auto head = node.second.get<int>("head");
                    BOOST_CHECK_EQUAL(head, obj.head);
                    auto data = node.second.get_child("data");
                    auto desc = data.get<int>("description");
                    BOOST_CHECK_EQUAL(desc, obj.desc);
        }
    }


BOOST_AUTO_TEST_SUITE_END()

#endif //JSON_STREAMER_WRITE_H
