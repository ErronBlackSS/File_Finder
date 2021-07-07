#pragma once
#include <stdexcept>
#include <string>

inline void error(const std::string& msg) {
    throw std::runtime_error(msg);
}