#include <iostream>
#include <random>

#include <Magick++.h>

#include "shape.hpp"
#include "image.hpp"

const uint CANVAS_WIDTH = 1920;
const uint CANVAS_HEIGHT = 1080;

int main(int argc, char* argv[]) {
    Magick::InitializeMagick(nullptr);
    RandomImage img(RandomImageConfiguration{argc, argv});
    img.write();
}