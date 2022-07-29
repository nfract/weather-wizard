#include "App.h"

#include "Decoder.h"

#include <iostream>

App::App(GLFWwindow* windowHandle, const AppConfig& config)
	: windowHandle(windowHandle), config(config), currentAction(AppGuiAction::NONE), recentStationSearchIndex(0)
{
	std::memset(stationCodeBuffer, 0, 12);
}

App::~App()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void App::Initialize()
{
	// Initiallize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ioHandle = &ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(windowHandle, true);
	ImGui_ImplOpenGL3_Init(config.shaderVersion.c_str());
	std::cout << "DEBUG: Initialized imgui\n";

	// Initialize required data sets
	std::cout << "Required data sets\n";

	// Stream in station data
	std::cout << "> prcp-inventory\n";
	Decoder::StreamStationNames("data-sets/prcp-inventory.txt", [&](const Station& station) {
		stations.Insert(station.code, station);

	});

	// Stream in precipitation data for red black tree
	std::cout << "> mly-prcp-normal\n";
	Decoder::StreamPrecipitationNormal("data-sets/mly-prcp-normal.txt", [&](const PrecipitationNormal& normal) {
	});
}

void App::Update()
{
	// Create a new imgui frame then render
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	UpdateMenuBar();
	HandleActions();

	ImGui::Begin("Station Code Finder");
	ImGui::Text("*Most recent search result will be highlighted green.");
	ImGui::Separator();
	ImGui::Text("Station Code:");
	ImGui::InputText("##Station Code: ", stationCodeBuffer, IM_ARRAYSIZE(stationCodeBuffer));
	if (ImGui::Button("Search..."))
	{
		std::string stationCodeBufferStr = std::string(stationCodeBuffer);

		intmax_t index = stations.Find(stationCodeBufferStr);
		if (index != -1)
		{
			stationSearchResults.emplace_back(stations.Search(index));
			recentStationSearchIndex = stationSearchResults.size() - 1;
		}

		std::memset(stationCodeBuffer, 0, 12);
	}

	ImGui::Separator();

	// Draw station search result table
	const ImGuiTableFlags flags
		= ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV
		| ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	ImGui::Text("Search History:");
	const int columnCount = 3;
	const char* columnNames[] = { "Station Code", "State", "Description"};
	if (ImGui::BeginTable("Test", 3, flags))
	{
		for (size_t i = 0; i < columnCount; i++)
		{
			ImGui::TableNextColumn();
			ImGui::PushID(i);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("%s", columnNames[i]);
			ImGui::Spacing();

			ImGui::PopID();
		}

		// Display station search history
		for (long long i = stationSearchResults.size() - 1; i >= 0; i--)
		{
			const Station station = stationSearchResults[i];

			// Highlight most recent search in green
			if (recentStationSearchIndex == i)
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(station.code.c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(station.state.c_str());
			ImGui::TableSetColumnIndex(2);
			ImGui::Text(station.description.c_str());

			if (recentStationSearchIndex == i)
				ImGui::PopStyleColor();
		}

		ImGui::EndTable();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::UpdateMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Show Station Code Finder"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void App::HandleActions()
{
}