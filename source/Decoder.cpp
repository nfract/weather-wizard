#include "Decoder.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#define STATE_STATION_CODE_INDEX 0
#define STATE_ABBR_CODE_INDEX    4
#define STATE_DESCRIPTION_INDEX  5
#define PRECIP_MONTH_BEGIN       1
#define PRECIP_MONTH_END         12

void Decoder::StreamStationNames(const std::string& filePath, std::function<void(const Station&)> CaptureFunction)
{
	std::string fileLine;
	std::ifstream fileHandle(filePath);

	if (!fileHandle.is_open())
	{
		std::cout << "Error: Could not open file " << filePath << ". Make sure it exists\n";
		return;
	}

	int indexer = 0;

	while (std::getline(fileHandle, fileLine, '\n'))
	{
		std::istringstream stream(fileLine);
		std::string streamLine;
		int indexer = 0;

		Station station;
		while (std::getline(stream, streamLine, ' '))
		{
			if (indexer == STATE_STATION_CODE_INDEX)
				station.code = streamLine;

			if (indexer == STATE_ABBR_CODE_INDEX)
				station.state = streamLine;

			if (indexer >= STATE_DESCRIPTION_INDEX)
				station.description = station.description + streamLine + " ";

			indexer++;
		}

		CaptureFunction(station);
	}

	fileHandle.close();
}

void Decoder::StreamPrecipitationNormal(const std::string& filePath, std::function<void(const PrecipitationNormal&)> CaptureFunction)
{
	std::string fileLine;
	std::ifstream fileHandle(filePath);

	if (!fileHandle.is_open())
	{
		std::cout << "Error: Could not open file " << filePath << ". Make sure it exists\n";
		return;
	}

	int indexer = 0;

	while (std::getline(fileHandle, fileLine, '\n'))
	{
		std::istringstream stream(fileLine);
		std::string streamLine;
		int indexer = 0;

		PrecipitationNormal precipitationNormal;
		while (std::getline(stream, streamLine, ' '))
		{
			if (indexer == STATE_STATION_CODE_INDEX)
				precipitationNormal.stationCode = streamLine;

			if (indexer >= PRECIP_MONTH_BEGIN)
			{
				// Fancey code to remove the flag characters from the data
				streamLine.erase(std::remove_if(streamLine.begin(), streamLine.end(), [&](char character) {
					switch (character) 
					{
					case 'C':
					case 'S':
					case 'R':
					case 'P':
					case 'Q':
						return true;
					default:
						return false;
					};
				}), streamLine.end());

				// -7777 means a close to zero value for that month so we can simply just 0 it
				precipitationNormal.normalAverageByMonth[indexer - 1] = streamLine != "-7777" ? std::stoi(streamLine) / 100.0f : 0;
			}

			indexer++;
		}

		// Calculate the average
		for (size_t i = 0; i < 12; i++)
			precipitationNormal.normalAverageForYear += precipitationNormal.normalAverageByMonth[i];
		precipitationNormal.normalAverageForYear /= 12.0f;

		CaptureFunction(precipitationNormal);
	}

	fileHandle.close();
}