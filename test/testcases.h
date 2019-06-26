//
// Created by mjonsson on 6/26/19.
//

#ifndef JSON_STREAMER_TESTCASES_H
#define JSON_STREAMER_TESTCASES_H

#define BOOST_TEST_DYN_LINK
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <forward_list>
#include <boost/test/unit_test.hpp>
#include "../src/json_streamer.h"
#include <string>

struct F {

    std::string path;
    std::forward_list<std::string> _keys { "head", "id", "description", "value" };

    F() {
        BOOST_TEST_MESSAGE( "setup fixture" );
        boost::filesystem::path cwd(boost::filesystem::current_path());
        path = cwd.string() + "/cache.json";
    }
    ~F()         {
        BOOST_TEST_MESSAGE( "teardown fixture" );
        if (boost::filesystem::exists(path))
            boost::filesystem::remove(path);
    }
};


BOOST_FIXTURE_TEST_SUITE(readwrite, F)

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

        JsonStreamer<Object> streamer(path, _keys);
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

        JsonStreamer<Object> streamer(path, _keys);
        streamer.write(obj);

        std::string test_string = "invalid";
        std::optional<Object> rsp = streamer.fetch(test_string);
        BOOST_CHECK_NE(rsp.has_value(), true);

        test_string = "parent";
        rsp = streamer.fetch(test_string);
        BOOST_CHECK_EQUAL(rsp.has_value(), true);
        BOOST_CHECK_EQUAL(rsp.value().desc, "testing");
    }

    BOOST_AUTO_TEST_CASE(read_second_string) {

        struct Object {
            std::string head;
            std::string id;
            std::string desc;
            std::string value;
        } obj;


        JsonStreamer<Object> streamer(path, _keys);

        obj.head = "parent";
        obj.id = "1";
        obj.desc = "testing";
        obj.value = "34";

        streamer.write(obj);

        obj.head = "parent4";
        obj.id = "2";
        obj.desc = "testing4";
        obj.value = "67";

        streamer.write(obj);

        std::string test_string = "parent4";
        std::optional<Object> rsp = streamer.fetch(test_string);
        BOOST_CHECK_EQUAL(rsp.has_value(), true);
        BOOST_CHECK_EQUAL(rsp.value().desc, "testing4");
    }

BOOST_AUTO_TEST_SUITE_END()


#endif //JSON_STREAMER_TESTCASES_H
