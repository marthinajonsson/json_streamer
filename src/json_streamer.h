//
// Created by mjonsson on 6/22/19.
//

#ifndef JSON_STREAMER_H
#define JSON_STREAMER_H


#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <forward_list>
#include <iostream>
#include <string>


#define LOG(x) {std::cout << __LINE__ << ": " << x << std::endl;}


template <class Object, class Type = std::forward_list<std::string>, class ValueType = std::string>
class JsonStreamer {
private:
    Type _keys;
    std::string _path;

    template <typename S> ValueType* get_begin(S *s)
    {
        return (ValueType*)s;
    }

    template <typename S> ValueType* get_end(S *s)
    {
        return (ValueType*)((ValueType*)s+sizeof(*s));
    }

    bool exists() {
        if (!boost::filesystem::exists(_path)) {
            boost::property_tree::ptree empty;
            std::fstream file;
            file.open(_path, std::ios::out | std::ios::app);
            if (file.is_open()) {
                LOG(_path + " created");
                boost::property_tree::write_json(file, empty);
                file.close();
                return true;
            }
            return false;
        }
        return true;
    }

    std::pair<ValueType, boost::property_tree::ptree> read(ValueType& search_pattern) noexcept
    {
        ValueType emptyVal;
        boost::property_tree::ptree root;
        boost::property_tree::ptree empty;

        if (! exists())
            LOG("File does not exists");

        boost::property_tree::read_json(_path, root);

        for (auto &it : root) {
            auto tt = it;
            auto val = it.second;
            if (val.empty())
                continue;

            auto found = val.get<std::string>(_keys.front());
            if (std::string::npos != found.find(search_pattern)) {
                search_pattern = found;
                return it;
            }
        }
        return {emptyVal, empty};
    }

public:

    JsonStreamer (std::string const &path, Type &keys) : _keys(keys), _path(path) {};
    ~JsonStreamer() = default;

    Object fetch(ValueType &search_pattern) noexcept
    {
        Object obj;
        Object* obj_ptr;
        obj_ptr = &obj;

        auto item = read(search_pattern);
        boost::property_tree::ptree tree = item.second.get_child("");
        auto key_ptr = _keys.begin();
        auto parent = tree.get<ValueType>(*key_ptr);
        ValueType *elem_ptr = get_begin(obj_ptr);
        *elem_ptr = parent;
        elem_ptr++;
        key_ptr++;

        auto data = tree.get_child("data");
        for ( ;elem_ptr < get_end(obj_ptr) && key_ptr != _keys.end(); elem_ptr++, key_ptr++) {
            auto val = *key_ptr;
            *elem_ptr = data.get<ValueType>(val);
        }

        return obj;
    }

    void write (Object obj) noexcept
    {
        boost::property_tree::ptree root;
        boost::property_tree::ptree parent;
        boost::property_tree::ptree child;

        if (! exists())
            LOG("File does not exists");

        boost::property_tree::read_json(_path, root);
        long size = root.size();
        auto index = std::to_string(size);

        auto key_ptr = _keys.begin();
        ValueType* value_ptr = get_begin(&obj);
        parent.put(*key_ptr, *value_ptr);
        key_ptr++;
        value_ptr++;

        for (;value_ptr < get_end(&obj) && key_ptr != _keys.end();value_ptr++, key_ptr++) {
            child.put(*key_ptr, *value_ptr);
        }

        parent.put_child("data", child);

        root.push_back(std::make_pair(index, parent));
        boost::property_tree::json_parser::write_json(_path, root);
    }
};


#endif //JSON_STREAMER_H
