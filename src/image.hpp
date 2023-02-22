#pragma once

#include <Magick++.h>
#include <sstream>

#include "shape.hpp"
#include "settings.hpp"

const std::string SETTINGS_FILE = "/etc/image-gen/settings";
auto ImageGenSettings = Settings(SETTINGS_FILE);

const size_t DEFAULT_CANVAS_WIDTH{1920};
const size_t DEFAULT_CANVAS_HEIGHT{1080};
const size_t DEFAULT_NUM_SHAPES{1};
const std::string DEFAULT_OUTPUT_FILE{"test.png"};
const std::string DEFAULT_RESOLUTION{"72x72"};

const std::string WIDTH_FLAG = "--width";
const std::string HEIGHT_FLAG = "--height";
const std::string RESOLUTION_FLAG = "--resolution";
const std::string NUM_SHAPES_FLAG = "--num_shapes";
const std::string OUTPUT_FILE_FLAG = "--out";

const std::string CanvasWidthKey = "Width";
const std::string CanvasHeightKey = "Height";
const std::string ResolutionKey = "Resolution";
const std::string NumShapesKey = "NumShapes";
const std::string OutKey = "Out";

size_t CanvasWidth()
{
    const auto &got = ImageGenSettings.param(CanvasWidthKey);
    if (got == "")
        return DEFAULT_CANVAS_WIDTH;
    return StringToT<size_t>(got);
}

size_t CanvasHeight()
{
    const auto &got = ImageGenSettings.param(CanvasHeightKey);
    if (got == "")
        return DEFAULT_CANVAS_HEIGHT;
    return StringToT<size_t>(got);
}

std::string Resolution()
{
    const auto &got = ImageGenSettings.param(ResolutionKey);
    if (got == "")
        return DEFAULT_RESOLUTION;
    return got;
}

size_t NumShapes()
{
    const auto &got = ImageGenSettings.param(NumShapesKey);
    if (got == "")
        return DEFAULT_NUM_SHAPES;
    return StringToT<size_t>(got);
}

std::string Out()
{
    const auto &got = ImageGenSettings.param(OutKey);
    if (got == "")
        return DEFAULT_OUTPUT_FILE;
    return got;
}

std::string usage()
{
    std::stringstream buffer;
    buffer << "Usage: ./image_gen\n";
    buffer << "Optional Flags:\n\t--help\tprint usage info\n\t";
    buffer << WIDTH_FLAG << "\tset output canvas width (default == " << DEFAULT_CANVAS_WIDTH << ")\n\t"
           << HEIGHT_FLAG << "\tset output canvas height (default == " << DEFAULT_CANVAS_HEIGHT << ")\n\t"
           << RESOLUTION_FLAG << "\tset output image resolution (default == " << DEFAULT_RESOLUTION << ")\n\t"
           << NUM_SHAPES_FLAG << "\tset number of shapes in output image (default == " << DEFAULT_NUM_SHAPES << ")\n\t"
           << OUTPUT_FILE_FLAG << "\tset output file name (default == " << DEFAULT_OUTPUT_FILE << ")\n\t"
           << std::endl;
    return buffer.str();
}

bool HasPrefix(std::string s, unsigned char c)
{
    return (s.front() == c);
}

std::string TrimAllPrefixChar(std::string s, unsigned char c)
{
    while (s.front() == c)
        s.erase(0, 1);
    return s;
}

struct RandomImageConfiguration
{
    RandomImageConfiguration() = default;
    RandomImageConfiguration(int argc, char *argv[]);
    size_t canvas_width{CanvasWidth()};
    size_t canvas_height{CanvasHeight()};
    size_t num_shapes{NumShapes()};
    Magick::Geometry resolution{Resolution()};
    std::string output_file{Out()};
    friend std::ostream &operator<<(std::ostream &os, const RandomImageConfiguration &configuration);
    friend bool operator==(const RandomImageConfiguration &lhs, const RandomImageConfiguration &rhs);
};

bool operator==(const RandomImageConfiguration &lhs, const RandomImageConfiguration &rhs)
{
    return (
        (lhs.canvas_width == rhs.canvas_width) &&
        (lhs.canvas_height == rhs.canvas_height) &&
        (lhs.num_shapes == rhs.num_shapes) &&
        (lhs.output_file == rhs.output_file));
}

std::ostream &operator<<(std::ostream &os, const RandomImageConfiguration &rhs)
{
    os << "Dimensions: " << rhs.canvas_width << "x" << rhs.canvas_height << "\n";
    os << "Resolution: " << rhs.resolution.width() << "x" << rhs.resolution.height() << "\n";
    os << "NumShapes: " << rhs.num_shapes << "\n";
    os << "Output Filename: " << rhs.output_file;
    return os;
}

RandomImageConfiguration::RandomImageConfiguration(int argc, char *argv[])
{
    for (auto i = 1; i < argc; i++)
    {
        const auto flag = std::string(argv[i]);
        if (flag == "--help" || flag == "-h")
        {
            std::cout << usage() << std::endl;
            exit(0);
        }
        if (i + 1 > argc)
        {
            throw std::string("RandomImageConfiguration::RandomImageConfiguration error: no argument after " + flag + ".\n" + usage());
        }
        i++;
        const auto value = std::string(argv[i]);
        if (flag == WIDTH_FLAG)
        {
            const auto value_t = StringToT<size_t>(value);
            canvas_width = value_t;
        }
        else if (flag == HEIGHT_FLAG)
        {
            const auto value_t = StringToT<size_t>(value);
            canvas_height = value_t;
        }
        else if (flag == NUM_SHAPES_FLAG)
        {
            const auto value_t = StringToT<size_t>(value);
            num_shapes = value_t;
        }
        else if (flag == OUTPUT_FILE_FLAG)
        {
            output_file = value;
        }
        else if (flag == RESOLUTION_FLAG)
        {
            resolution = Magick::Geometry(value);
        }
        else
        {
            throw std::string("RandomImageConfiguration::RandomImageConfiguration error: unknown flag " + flag + " found with value " + value + ".\n" + usage());
        }
    }
}

struct RandomImage : public Magick::Image
{
    RandomImage() = default;
    RandomImage(const RandomImageConfiguration &);
    void write();
    const RandomImageConfiguration &config() const { return configuration; }

private:
    Magick::Image image;
    RandomImageConfiguration configuration{};
};

void RandomImage::write()
{
    image.write(configuration.output_file);
}

RandomImage::RandomImage(const RandomImageConfiguration &configuration) : configuration{configuration}
{
    Magick::Geometry geometry(configuration.canvas_width, configuration.canvas_height);
    const auto color = RandomColor();
    image = Magick::Image(geometry, color);
    image.density(configuration.resolution);
    for (size_t iteration = 0; iteration < configuration.num_shapes; iteration++)
    {
        image.draw(RandomDrawAction(configuration.canvas_width, configuration.canvas_height));
    }
}