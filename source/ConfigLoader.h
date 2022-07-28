#pragma once

#include <string>

struct Config
{

};

class ConfigLoader
{
public:
	static Config LoadConfig(const std::string& filePath);
};