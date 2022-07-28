#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <unordered_map>
#include <string>

#include "formats/Station.h"

enum class AppGuiAction
{
	NONE,
	MAIN_MENU_BAR_STATION_CODE_FINDER
};

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
	void UpdateMenuBar();
	void HandleActions();

private:
	// Application Window Data
	GLFWwindow* windowHandle;
	ImGuiIO* ioHandle;

	AppConfig config;

	// Application Datas
	std::unordered_map<std::string, Station> stationMap;

	// Gui data
	AppGuiAction currentAction;
	char stationCodeBuffer[12];
	size_t recentStationSearchIndex;
	std::vector<Station*> stationSearchResults;
};