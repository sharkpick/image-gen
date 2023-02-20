#pragma once

#include <Magick++.h>
#include <random>

Magick::ColorRGB RandomColor() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribution(0.0, 1.0);
    const auto red = distribution(gen);
    const auto green = distribution(gen);
    const auto blue = distribution(gen);
    return Magick::ColorRGB(red, green, blue);
}

uint RandomStrokeWidth() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distribution(1, 25);
    return distribution(gen);
}