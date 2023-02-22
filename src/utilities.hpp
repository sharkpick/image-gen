#pragma once

#include <Magick++.h>
#include <random>
#include <sstream>

std::vector<std::string> SplitString(std::string s, std::string delimiter)
{
    std::vector<std::string> results{};
    size_t position = 0;
    while ((position = s.find(delimiter)) != std::string::npos)
    {
        const auto buffer = s.substr(0, position);
        results.push_back(buffer);
        s.erase(0, position + delimiter.length());
    }
    if (s.length() > 0)
        results.push_back(s);
    return results;
}

template <typename T>
T Random(T min, T max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<T> distr(min, max);
    return distr(generator);
}

template <typename T>
T RandomReal(T min, T max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<T> distr(min, max);
    return distr(generator);
}

Magick::ColorRGB RandomColor()
{
    const auto red = RandomReal<double>(0.0, 1.0);
    const auto green = RandomReal<double>(0.0, 1.0);
    const auto blue = RandomReal<double>(0.0, 1.0);
    return Magick::ColorRGB(red, green, blue);
}

uint RandomStrokeWidth()
{
    const uint min = 1;
    const uint max = 25;
    return Random<uint>(min, max);
}