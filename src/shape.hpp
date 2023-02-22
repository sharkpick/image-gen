#pragma once

#include <iostream>
#include <Magick++.h>

#include "utilities.hpp"

struct Coordinates
{
    double x{};
    double y{};
    friend std::ostream &operator<<(std::ostream &os, Coordinates &rhs);
};

std::ostream &operator<<(std::ostream &os, Coordinates &rhs)
{
    os << rhs.x << "x" << rhs.y;
    return os;
}

Magick::DrawableRectangle RandomRectangle(const uint CANVAS_WIDTH, const uint CANVAS_HEIGHT)
{
    const auto start_x = Random<uint>(CANVAS_HEIGHT * .1, CANVAS_HEIGHT * .9);
    const auto start_y = Random<uint>(start_x, CANVAS_WIDTH * .9);
    const auto stop_x = Random<uint>(start_x, CANVAS_HEIGHT * .9);
    const auto stop_y = Random<uint>(start_y, CANVAS_WIDTH * .9);
    return Magick::DrawableRectangle(start_x, start_y, stop_x, stop_y);
}

Magick::DrawableCircle RandomCircle(const uint CANVAS_WIDTH, const uint CANVAS_HEIGHT)
{
    /*const auto start_x = Random<uint>(0, CANVAS_HEIGHT);
    const auto start_y = Random<uint>(start_x, CANVAS_WIDTH);
    const auto stop_x = Random<uint>(start_x, CANVAS_HEIGHT);
    const auto stop_y = Random<uint>(start_y, CANVAS_WIDTH);*/
    const auto start_x = Random<uint>(CANVAS_HEIGHT * .1, CANVAS_HEIGHT * .9);
    const auto start_y = Random<uint>(start_x, CANVAS_WIDTH * .9);
    const auto stop_x = Random<uint>(start_x, CANVAS_HEIGHT * .9);
    const auto stop_y = Random<uint>(start_y, CANVAS_WIDTH * .9);
    return Magick::DrawableCircle(start_x, start_y, stop_x, stop_y);
}

std::list<Magick::Drawable> RandomDrawAction(const uint CANVAS_WIDTH, const uint CANVAS_HEIGHT)
{
    std::list<Magick::Drawable> action{};
    action.push_back(Magick::DrawableStrokeColor(RandomColor()));
    action.push_back(Magick::DrawableStrokeWidth(RandomStrokeWidth()));
    action.push_back(Magick::DrawableFillColor(RandomColor()));
    const auto shape = Random<uint>(1, 2);
    switch (shape)
    {
    case 1:
        action.push_back(RandomRectangle(CANVAS_WIDTH, CANVAS_HEIGHT));
        break;
    case 2:
        action.push_back(RandomCircle(CANVAS_WIDTH, CANVAS_HEIGHT));
        break;
    };
    return action;
}