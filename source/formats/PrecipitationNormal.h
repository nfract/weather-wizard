#pragma once

#include <array>
#include <string>

struct PrecipitationNormal
{
	std::string stationCode;

	float normalAverageForYear;
	std::array<float, 12> normalAverageByMonth;
};