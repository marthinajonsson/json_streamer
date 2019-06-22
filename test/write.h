//
// Created by mjonsson on 6/22/19.
//

#ifndef JSON_STREAMER_WRITE_H
#define JSON_STREAMER_WRITE_H

#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <forward_list>
#include <string>

#include "json_streamer.h"

class WriteTest : public ::testing::Test
{
protected:
    std::forward_list<std::string> _keys { "head", "id", "description", "value" };
    std::string path;
    void SetUp() override
    {
        boost::filesystem::path cwd(boost::filesystem::current_path());
        path = cwd.string() + "/cache.json";
    }

    void TearDown() override
    {
        if (boost::filesystem::exists(path))
            boost::filesystem::remove(path);
    }
};

TEST_F(WriteTest, test1) {

    struct Object {
        std::string head;
        std::string id;
        std::string desc;
        std::string value;
    }obj;

    obj.head = "parent";
    obj.id = "1";
    obj.desc = "testing";
    obj.value = "34";

    JsonStreamer<Object> streamer (path, _keys);
    streamer.write(obj);

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(path, pt);
    pt = pt.get_child("");

    BOOST_FOREACH(boost::property_tree::ptree::value_type& node , pt) {
                    auto head = node.second.get<std::string>("head");
                    ASSERT_EQ(head, obj.head);
                    auto data = node.second.get_child("data");
                    auto desc = data.get<std::string>("description");
                    ASSERT_EQ(desc, obj.desc);
                }
}

TEST_F(WriteTest, test2) {

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

    JsonStreamer<Object, std::forward_list<std::string>, int> streamer (path, _keys);
    streamer.write(obj);

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(path, pt);
    pt = pt.get_child("");

    BOOST_FOREACH(boost::property_tree::ptree::value_type& node , pt) {
                    auto head = node.second.get<int>("head");
                    ASSERT_EQ(head, obj.head);
                    auto data = node.second.get_child("data");
                    auto desc = data.get<int>("description");
                    ASSERT_EQ(desc, obj.desc);
                }
}

#endif //JSON_STREAMER_WRITE_H
