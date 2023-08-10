#pragma once

#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

std::string strip(const std::string& str) {
    std::string result;
    int firstNonSpace = -1, lastNonSpace = -1;
    for(int i = 0; i < str.size(); i++) {
        if(!std::isspace(str[i])) {
            if(firstNonSpace == -1) {
                firstNonSpace = i;
            }
            lastNonSpace = i;
        }
    }
    if(firstNonSpace == -1 || lastNonSpace == -1) {
        return "";
    }
    return str.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
}

template <typename Container, typename Value>
bool find(const Container& container, const Value& value) {
    return std::find(container.begin(), container.end(), value) != container.end();
}

template <typename K, typename V>
bool find(const std::unordered_map<K, V> &container, const K &key) {
    return container.find(key) != container.end();
}

template <typename Container, typename Value>
bool not_find(const Container& container, const Value& value) {
    return std::find(container.begin(), container.end(), value) == container.end();
}

template <typename K, typename V>
bool not_find(const std::unordered_map<K, V> &container, const K &key) {
    return container.find(key) == container.end();
}