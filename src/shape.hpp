#pragma once

#include <iostream>

#include <Magick++.h>
#include <random>

#include "utilities.hpp"

struct Coordinates {
    double x{};
    double y{};
    friend std::ostream& operator<<(std::ostream& os, Coordinates& rhs);
};

std::ostream& operator<<(std::ostream& os, Coordinates& rhs) {
    os << rhs.x << "x" << rhs.y;
    return os;
}

Magick::DrawableRectangle RandomRectangle(const uint CANVAS_WIDTH, const uint CANVAS_HEIGHT) {
    std::random_device rd;
    std::mt19937 gen(rd());
    Coordinates start_coordinates{};
    Coordinates end_coordinates{};
    std::uniform_int_distribution<> start_x_distribution(0, CANVAS_HEIGHT);
    start_coordinates.x = start_x_distribution(gen);
    std::uniform_int_distribution<> start_y_distribution(start_coordinates.x, CANVAS_HEIGHT*.95);
    start_coordinates.y = start_y_distribution(gen);
    //std::cout << "start: " << start_coordinates << std::endl;
    std::uniform_int_distribution<> end_x_distribution(start_coordinates.x, CANVAS_HEIGHT);
    end_coordinates.x = end_x_distribution(gen);
    std::uniform_int_distribution<> end_y_distribution(start_coordinates.y, CANVAS_HEIGHT);
    end_coordinates.y = end_y_distribution(gen);
    //std::cout << "end: " << end_coordinates << std::endl;
    return Magick::DrawableRectangle(start_coordinates.x, start_coordinates.y, end_coordinates.x, end_coordinates.y);
}

Magick::DrawableCircle RandomCircle(const uint CANVAS_WIDTH, const uint CANVAS_HEIGHT) {
    std::random_device rd;
    std::mt19937 gen(rd());
    Coordinates start_coordinates{};
    Coordinates end_coordinates{};
    std::uniform_int_distribution<> start_x_distribution(0, CANVAS_HEIGHT*.75);
    start_coordinates.x = start_x_distribution(gen);
    std::uniform_int_distribution<> start_y_distribution(start_coordinates.x, CANVAS_HEIGHT*.75);
    start_coordinates.y = start_y_distribution(gen);
    //std::cout << "start: " << start_coordinates << std::endl;
    std::uniform_int_distribution<> end_x_distribution(start_coordinates.x, CANVAS_WIDTH);
    end_coordinates.x = end_x_distribution(gen);
    std::uniform_int_distribution<> end_y_distribution(start_coordinates.y, CANVAS_HEIGHT);
    end_coordinates.y = end_y_distribution(gen);
    //std::cout << "end: " << end_coordinates << std::endl;
    return Magick::DrawableCircle(start_coordinates.x, start_coordinates.y, end_coordinates.x, end_coordinates.y);
}

std::random_device shape_rd;
std::mt19937 shape_gen(shape_rd());
std::uniform_int_distribution<> shape_distribution(1, 2);

std::list<Magick::Drawable> RandomDrawAction(const uint CANVAS_WIDTH, const uint CANVAS_HEIGHT) {
    std::list<Magick::Drawable> action{};
    action.push_back(Magick::DrawableStrokeColor(RandomColor()));
    action.push_back(Magick::DrawableStrokeWidth(RandomStrokeWidth()));
    action.push_back(Magick::DrawableFillColor(RandomColor()));
    const auto shape = shape_distribution(shape_gen);
    switch (shape) {
        case 1:
            action.push_back(RandomRectangle(CANVAS_WIDTH, CANVAS_HEIGHT));
            break;
        case 2:
            action.push_back(RandomCircle(CANVAS_WIDTH, CANVAS_HEIGHT));
            break;
    };
    return action;
}