#pragma once

#include "formats/PrecipitationNormal.h"
#include "formats/Station.h"

#include <functional>
#include <string>
#include <array>

class Decoder
{
public:
	static void StreamStationNames(const std::string& filePath, std::function<void(const Station&)> CaptureFunction);
	static void StreamPrecipitationNormal(const std::string& filePath, std::function<void(const PrecipitationNormal&)> CaptureFunction);
};