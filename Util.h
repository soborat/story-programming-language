#pragma once

#include <sstream>
#include <vector>
#include <string>
#include <memory>

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

bool find(const std::initializer_list<std::string>& init_list, const std::string& value) {
    return std::find(init_list.begin(), init_list.end(), value) == init_list.end();
}

template <typename K, typename V>
bool find(const std::unordered_map<K, V> &container, const K &key) {
    return container.find(key) != container.end();
}

template <typename Container, typename Value>
bool not_find(const Container& container, const Value& value) {
    return std::find(container.begin(), container.end(), value) == container.end();
}

bool not_find(const std::initializer_list<std::string>& init_list, const std::string& value) {
    return std::find(init_list.begin(), init_list.end(), value) == init_list.end();
}

template <typename K, typename V>
bool not_find(const std::unordered_map<K, V> &container, const K &key) {
    return container.find(key) == container.end();
}

template <typename T>
T fmt_arg(T t) {
    return t;
}

const char* fmt_arg(const std::string& s) {
    return s.c_str();
}

template <typename... Args>
std::string fmt(const char* format, Args... args) {
    size_t size = snprintf(nullptr, 0, format, fmt_arg(args)...) + 1;
    std::unique_ptr<char[]> buffer(new char[size]);

    snprintf(buffer.get(), size, format, fmt_arg(args)...);
    return {buffer.get(), buffer.get() + size - 1};
}

std::string repeatString(const std::string& str, int times) {
    std::string result;
    for(int i = 0; i < times; i++) {
        result += str;
    }
    return result;
}