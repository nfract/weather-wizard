#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <string>

#include "formats/PrecipitationNormal.h"
#include "formats/Station.h"
#include "datastructs/HashMap.h"

struct AppConfig
{
	int width;
	int height;
	std::string shaderVersion;
};

class App
{
public:
	App(GLFWwindow* windowHandle, const AppConfig& config);
	~App();

	void Initialize();
	void Update();

private:
	void SearchStation(const std::string& stationCode);

private:
	// Application Window Data
	GLFWwindow* windowHandle;
	ImGuiIO* ioHandle;

	AppConfig config;

	// Application Datas
	HashMap<Station> stations;

	// Gui data
	char stationCodeBuffer[12];
	size_t recentStationSearchIndex;
	std::vector<Station> stationSearchResults;

	std::string executionTime;
	int selectedDataStructure;
	int selectedMinOrMax;
	int itemsToFind;
	std::vector<PrecipitationNormal> precipitationSearchResults;
};