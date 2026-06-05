#pragma once

#include <string>
#include <random>

inline std::string GenerateRandomString(int length)
{
    static const std::string chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    std::random_device            rd;
    std::mt19937                  rng(rd());
    std::uniform_int_distribution dist(0, (int)chars.size() - 1);

    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; i++)
        result += chars[dist(rng)];

    return result;
}