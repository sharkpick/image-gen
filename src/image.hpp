#pragma once

#include <Magick++.h>
#include <sstream>

const std::string usage = "usage: ./image_gen --width 1920 --height 1080 --num_shapes 10 --out filename.jpg";

template <typename T>
T StringToT(std::string s) {
    std::stringstream buffer;
    buffer << s;
    T t;
    buffer >> t;
    return t;
}

bool HasPrefix(std::string s, unsigned char c) {
    return (s.front() == c);
}

std::string TrimAllPrefixChar(std::string s, unsigned char c) {
    while (s.front() == c) s.erase(0, 1);
    return s;
}

const size_t DEFAULT_CANVAS_WIDTH{1920};
const size_t DEFAULT_CANVAS_HEIGHT{1080};
const size_t DEFAULT_NUM_SHAPES{1};
const std::string DEFAULT_OUTPUT_FILE{"test.png"};

struct RandomImageConfiguration {
    RandomImageConfiguration() = default;
    RandomImageConfiguration(int argc, char* argv[]);
    RandomImageConfiguration(size_t canvas_width, size_t canvas_height, size_t num_shapes);
    size_t canvas_width{DEFAULT_CANVAS_WIDTH};
    size_t canvas_height{DEFAULT_CANVAS_HEIGHT};
    size_t num_shapes{DEFAULT_NUM_SHAPES};
    std::string output_file{DEFAULT_OUTPUT_FILE};
    friend std::ostream& operator<<(std::ostream& os, const RandomImageConfiguration& configuration);
};

std::ostream& operator<<(std::ostream& os, const RandomImageConfiguration& rhs) {
    os << "Dimensions: " << rhs.canvas_width << "x" << rhs.canvas_height << "\n";
    os << "NumShapes: " << rhs.num_shapes << "\n";
    os << "Output Filename: " << rhs.output_file;
    return os;
}

RandomImageConfiguration::RandomImageConfiguration(int argc, char* argv[]) {
    for (auto i = 1; i < argc; i++) {
        auto flag = std::string(argv[i]);
        if (HasPrefix(flag, '-')) {
            flag = TrimAllPrefixChar(flag, '-');
            if (i+1 < argc) {
                i++;
                const auto value_string = std::string(argv[i]);
                if (flag == "out") {
                    output_file = value_string;
                    continue;
                }                
                const auto value = StringToT<size_t>(value_string);
                if (flag == "width") {
                    canvas_width = value;
                } else if (flag == "height") {
                    canvas_height = value;
                } else if (flag == "num_shapes") {
                    num_shapes = value;
                } else {
                    throw std::string("error: unrecognized flag "+flag+"\n"+usage);
                }
            } else {
                throw usage;
            }
        }
    }
}

RandomImageConfiguration::RandomImageConfiguration(size_t canvas_width, size_t canvas_height, size_t num_shapes) 
    : canvas_width{canvas_width}, canvas_height{canvas_height}, num_shapes{num_shapes} {}

struct RandomImage : public Magick::Image {
    RandomImage() = default;
    RandomImage(const RandomImageConfiguration&);
    void write();
private:
    Magick::Image image;
    RandomImageConfiguration configuration{};
};

void RandomImage::write() {
    image.write(configuration.output_file);    
}

RandomImage::RandomImage(const RandomImageConfiguration& configuration) : configuration{configuration} {
    Magick::Geometry geometry(configuration.canvas_width, configuration.canvas_height);
    const auto color = RandomColor();
    image = Magick::Image(geometry, color);
    for (size_t iteration = 0; iteration < configuration.num_shapes; iteration++) {
        image.draw(RandomDrawAction(configuration.canvas_width, configuration.canvas_height));
    }
}