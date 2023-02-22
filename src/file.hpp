#pragma once

#include <filesystem>
#include <fstream>

template <typename T>
time_t to_time_t(T t)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(t - T::clock::now() + system_clock::now());
    return system_clock::to_time_t(sctp);
}

bool FileExists(std::string filename)
{
    return std::filesystem::exists(filename);
}

std::string ReadFile(std::string filename)
{
    size_t size{};
    try
    {
        size = std::filesystem::file_size(filename);
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        throw err;
    }
    std::vector<unsigned char> buffer;
    buffer.resize(size);
    std::ifstream file;
    file.exceptions(std::ifstream::badbit);
    try
    {
        file.open(filename);
        file.read((char *)buffer.data(), size);
        file.close();
    }
    catch (const std::ifstream::failure &err)
    {
        throw err;
    }
    return std::string(buffer.begin(), buffer.end());
}

void WriteFile(std::string filename, std::string contents)
{
    std::ofstream file;
    try
    {
        file.open(filename);
        file << contents;
        file.close();
    }
    catch (const std::ofstream::failure &err)
    {
        throw err;
    }
}

time_t FileLastModified(std::string filename)
{
    time_t modified;
    try
    {
        modified = to_time_t<std::filesystem::file_time_type>(std::filesystem::last_write_time(filename));
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        return 0;
    }
    return modified;
}