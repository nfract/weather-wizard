#pragma once

#include <string>

struct PrecipitationNormal
{
	std::string stationCode;

	float normalAverageForYear;
	float normalAverageByMonth[12] = {};
};