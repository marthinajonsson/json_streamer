//
// Created by mjonsson on 6/22/19.
//

#ifndef JSON_STREAMER_H
#define JSON_STREAMER_H


#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <forward_list>
#include <optional>
#include <iostream>
#include <string>


#define LOG(x) {std::cout << __LINE__ << ": " << x << std::endl;}


template <class Object, class Type = std::forward_list<std::string>>
class JsonStreamer {
private:
    Type _keys;
    std::string _path;

    template <typename S> std::string* get_begin(S *s)
    {
        return (std::string*)s;
    }

    template <typename S> std::string* get_end(S *s)
    {
        return (std::string*)((std::string*)s+sizeof(*s));
    }

    bool file_exists() {
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

    std::pair<std::string, boost::property_tree::ptree> find(std::string& search_pattern) noexcept
    {
        std::string empty_val;
        std::string use_string;
        boost::property_tree::ptree root;
        boost::property_tree::ptree empty;

        if (! file_exists())
            LOG("File does not exists");


        std::stringstream ss;
        ss << search_pattern;
        use_string = ss.str();

        boost::property_tree::read_json(_path, root);

        for (auto& it : root) {
            boost::property_tree::ptree val = it.second;
            if (val.empty())
                continue;

            auto readValueType = val.get<std::string>(_keys.front());

            std::stringstream ss2;
            ss2 << readValueType;
            std::string found = ss2.str();
            if (std::string::npos != found.find(use_string))
            {
                std::istringstream iss (it.first);
                std::string key;
                iss >> key;
                if (iss.fail())
                    LOG("Something might have gone wrong, check your result");

                return {key, val};
            }
        }
        return {empty_val, empty};
    }

public:

    JsonStreamer (std::string &path, Type &keys) : _keys(keys), _path(path) {};
    ~JsonStreamer() = default;

    void update (Object obj) noexcept
    {
        std::string* elem_ptr = get_begin(&obj);
        auto key_ptr = _keys.begin();
        key_ptr++;
        auto item = find(*elem_ptr);
        if (!item.second.empty()) {
            boost::property_tree::ptree node = item.second.get_child("");
            auto data = node.get_child("data");
            elem_ptr++;

            for (;elem_ptr < get_end(&obj) && key_ptr != _keys.end();elem_ptr++, key_ptr++) {
                //data.put_value(.....)
            }

        }
        return obj;
    }

    std::optional<Object> fetch(std::string &search_pattern) noexcept
    {
        Object obj;
        Object* obj_ptr;
        obj_ptr = &obj;

        auto item = find(search_pattern);
        if (item.second.empty())
            return std::nullopt;

        boost::property_tree::ptree node = item.second.get_child("");
        auto key_ptr = _keys.begin();
        std::string parent = node.get<std::string>(*key_ptr);
        std::string *elem_ptr = get_begin(obj_ptr);
        *elem_ptr = parent;
        elem_ptr++;
        key_ptr++;

        auto data = node.get_child("data");
        for ( ;elem_ptr < get_end(obj_ptr) && key_ptr != _keys.end(); elem_ptr++, key_ptr++) {
            auto val = *key_ptr;
            *elem_ptr = data.get<std::string>(val);
        }

        return obj;
    }

    boost::property_tree::ptree fetch_node(std::string &search_pattern) noexcept
    {
        auto item = find(search_pattern);
        if (item.second.empty())

        return item.second.get_child("").data();
    }

    void write (Object obj) noexcept
    {
        boost::property_tree::ptree root;
        boost::property_tree::ptree parent;
        boost::property_tree::ptree child;

        if (! file_exists())
            LOG("File does not exists");

        boost::property_tree::read_json(_path, root);
        long size = root.size();
        auto index = std::to_string(size);

        auto key_ptr = _keys.begin();
        std::string* value_ptr = get_begin(&obj);
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
