#pragma once

#include <map>
#include <mutex>
#include <filesystem>
#include <chrono>
#include <sstream>

#include "file.hpp"
#include "utilities.hpp"

template <typename T>
T StringToT(std::string s, bool isHex = false)
{
    std::stringstream buffer;
    if (isHex)
        buffer << std::hex;
    buffer << s;
    T t;
    buffer >> t;
    return t;
}

class Settings
{
public:
    Settings() = delete;
    Settings(std::string, std::vector<std::string>, bool);
    std::string filename() const;
    void filename(std::string);
    std::string param(std::string);
    void param(std::string, std::string);
    void update();
    void operator=(const Settings &rhs);
    Settings &logging(bool);
    bool logging() const;

private:
    void locked_update();
    time_t _last_modified{0};
    mutable std::mutex _mutex{};
    std::string _filename{};
    std::map<std::string, std::string> _contents{};
    bool _logging{true};
};

Settings &Settings::logging(bool doLogging)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    _logging = doLogging;
    return *this;
}

bool Settings::logging() const
{
    const std::lock_guard<std::mutex> lock(_mutex);
    return _logging;
}
void Settings::operator=(const Settings &rhs)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    const std::lock_guard<std::mutex> other(rhs._mutex);
    _last_modified = rhs._last_modified;
    _filename = rhs._filename;
    _contents = rhs._contents;
}

void Settings::update()
{
    locked_update();
}

void Settings::locked_update()
{
    const std::lock_guard<std::mutex> lock(_mutex);
    const auto file_last_modified = FileLastModified(_filename);
    const auto diff_time = difftime(file_last_modified, _last_modified);
    if (diff_time < 1)
        return;
    std::vector<std::string> file_contents;
    try
    {
        file_contents = SplitString(ReadFile(_filename), "\n");
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        std::cerr << "Settings::locked_update error getting file size: " << err.what() << std::endl;
        return;
    }
    catch (const std::ifstream::failure &err)
    {
        std::cerr << "Settings::locked_update error reading file: " << err.what() << std::endl;
        return;
    }
    for (const auto &row : file_contents)
    {
        if (row.length() == 0)
            continue;
        const auto splitIndex = row.find("=");
        if (splitIndex == std::string::npos)
        {
            std::cerr << "Settings::locked_update error: " << row << " is not a valid key=value pair\n";
            continue;
        }
        const auto key = row.substr(0, splitIndex);
        const auto value = row.substr(splitIndex + 1, row.length());
        const auto found = _contents.find(key);
        if (found != _contents.end() && found->second == value)
            continue;
        else if (found == _contents.end())
        {
            if (_logging)
                std::cout << "Settings::locked_update adding key " << key << " with value " << value << "\n";
        }
        else
        {
            if (_logging)
                std::cout << "Settings::locked_update updating value for key " << key << " from " << found->second << " to " << value << "\n";
        }
        _contents[key] = value;
    }
    _last_modified = file_last_modified;
}

std::string Settings::filename() const
{
    const std::lock_guard<std::mutex> lock(_mutex);
    return _filename;
}
void Settings::filename(std::string filename)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    _filename = filename;
}

std::string Settings::param(std::string key)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    const auto found = _contents.find(key);
    if (found == _contents.end())
        return "";
    return found->second;
}

void Settings::param(std::string key, std::string value)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    const auto found = _contents.find(key);
    if (found == _contents.end())
    {
        if (_logging)
            std::cout << "Settings::param() adding key " << key << " with value " << value << "\n";
    }
    else
    {
        if (_logging)
            std::cout << "Settings::param() updating key " << key << " value from " << found->second << " to " << value << "\n";
    }
    _contents[key] = value;
}

Settings::Settings(std::string filename, std::vector<std::string> optional_parameters = {}, bool doLogging = true)
    : _filename{filename}, _logging{doLogging}
{
    for (const auto &param : optional_parameters)
    {
        const auto splitPosition = param.find("=");
        if (splitPosition == std::string::npos)
        {
            std::cerr << "Settings::Settings() error: " << param << " is not a valid key=value pair\n";
            continue;
        }
        const auto key = param.substr(0, splitPosition);
        const auto value = param.substr(splitPosition + 1, param.length());
        if (_logging)
            std::cout << "Settings::Settings adding key " << key << " with value " << value << "\n";
        _contents[key] = value;
    }
    locked_update();
}