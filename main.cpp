#include <iostream>
#include <random>

#include <Magick++.h>

using namespace Magick;

const uint CANVAS_WIDTH = 1920;
const uint CANVAS_HEIGHT = 1080;

struct Coordinates {
    double x{};
    double y{};
    friend std::ostream& operator<<(std::ostream& os, Coordinates& rhs);
};

std::ostream& operator<<(std::ostream& os, Coordinates& rhs) {
    os << rhs.x << "x" << rhs.y;
    return os;
}

DrawableRectangle RandomRectangle() {
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
    return DrawableRectangle(start_coordinates.x, start_coordinates.y, end_coordinates.x, end_coordinates.y);
}

DrawableCircle RandomCircle() {
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
    return DrawableCircle(start_coordinates.x, start_coordinates.y, end_coordinates.x, end_coordinates.y);
}

ColorRGB RandomColor() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribution(0.0, 1.0);
    const auto red = distribution(gen);
    const auto green = distribution(gen);
    const auto blue = distribution(gen);
    return ColorRGB(red, green, blue);
}

uint RandomStrokeWidth() {
    const auto min = 1;
    const auto max = 25;
    return (rand() % (max-min)+1) + min;
}

std::random_device shape_rd;
std::mt19937 shape_gen(shape_rd());
std::uniform_int_distribution<> shape_distribution(1, 2);

std::list<Magick::Drawable> RandomDrawAction() {
    std::list<Magick::Drawable> action{};
    action.push_back(DrawableStrokeColor(RandomColor()));
    action.push_back(DrawableStrokeWidth(RandomStrokeWidth()));
    action.push_back(DrawableFillColor(RandomColor()));
    const auto shape = shape_distribution(shape_gen);
    switch (shape) {
        case 1:
            action.push_back(RandomRectangle());
            break;
        case 2:
            action.push_back(RandomCircle());
            break;
    };
    return action;
}

int main() {
    srand(time(nullptr));
    Magick::InitializeMagick(nullptr);
    Geometry geometry{};
    geometry.width(CANVAS_WIDTH);
    geometry.height(CANVAS_HEIGHT);
    Image image{geometry, RandomColor()};
    for (auto i = 0; i < 4; i++) {
        image.draw(RandomDrawAction());
    }
    image.write("test.png");
}