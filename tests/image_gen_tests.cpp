#include <gtest/gtest.h>
#include <filesystem>

#include "../src/image.hpp"

namespace fs = std::filesystem;

TEST(RandomImageConfiguration, RandomImageConfigurationConstructor) {
    auto config = RandomImageConfiguration{};
    EXPECT_EQ(config.canvas_width, DEFAULT_CANVAS_WIDTH);
    EXPECT_EQ(config.canvas_height, DEFAULT_CANVAS_HEIGHT);
    EXPECT_EQ(config.num_shapes, DEFAULT_NUM_SHAPES);
    EXPECT_EQ(config.output_file, DEFAULT_OUTPUT_FILE);
    config = RandomImageConfiguration(1280, 720, 5, "new_output.png");
    EXPECT_EQ(config.canvas_width, 1280);
    EXPECT_EQ(config.canvas_height, 720);
    EXPECT_EQ(config.num_shapes, 5);
    EXPECT_EQ(config.output_file, "new_output.png");
}

TEST(RandomImage, RandomImageConstructor) {
    const auto config = RandomImageConfiguration();
    auto image = RandomImage(config);
    EXPECT_EQ(image.config(), config);
}

TEST(RandomImage, RandomImageWrite) {
    const auto config = RandomImageConfiguration();
    auto image = RandomImage(config);
    EXPECT_EQ(image.config(), config);
    fs::remove(config.output_file);
    image.write();
    EXPECT_TRUE(fs::exists(config.output_file));
    const auto got_image = Magick::Image(config.output_file);
    EXPECT_EQ(got_image.magick(), "PNG");
    fs::remove(config.output_file);
}

int main(int argc, char* argv[]) {
    Magick::InitializeMagick(nullptr);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}