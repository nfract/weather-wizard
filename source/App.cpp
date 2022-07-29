#include "App.h"

#include "Decoder.h"

#include <iostream>

App::App(GLFWwindow* windowHandle, const AppConfig& config)
	: windowHandle(windowHandle), config(config), recentStationSearchIndex(0), selectedDataStructure(0), itemsToFind(10), executionTime("0 microseconds")
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

	int id = 0;

	// Stream in precipitation data for red black tree
	std::cout << "> mly-prcp-normal\n";
	Decoder::StreamPrecipitationNormal("data-sets/mly-prcp-normal.txt", [&](const PrecipitationNormal& normal) {
		if (id > 200) return;
		
		precipitationSearchResults.emplace_back(normal);

		id++;
	});
}

void App::Update()
{
	// Create a new imgui frame then render
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Station code finder window
	ImGui::Begin("Station Code Finder");
	ImGui::Text("*Most recent search result will be highlighted green.");
	ImGui::Separator();
	ImGui::Text("Station Code");
	ImGui::InputText("##Station Code", stationCodeBuffer, IM_ARRAYSIZE(stationCodeBuffer));
	if (ImGui::Button("Search..."))
	{
		std::string stationCodeBufferStr = std::string(stationCodeBuffer);
		SearchStation(stationCodeBufferStr);
	}

	ImGui::Separator();

	// Draw station search result table
	const ImGuiTableFlags flags
		= ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV
		| ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	ImGui::Text("Search History");
	const int columnCountSearchHistory = 3;
	const char* columnNamesSearchHistory[] = { "Station Code", "State", "Description"};
	if (ImGui::BeginTable("Search History", 3, flags))
	{
		for (size_t i = 0; i < columnCountSearchHistory; i++)
		{
			ImGui::TableNextColumn();
			ImGui::PushID(i);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("%s", columnNamesSearchHistory[i]);
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

	//---------------------------------------------------------------------------------------

	//Kth largest window
	ImGui::Begin("Kth Largest");
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 123, 232, 255));
	ImGui::Text("*Execution time");
	ImGui::Text(executionTime.c_str());
	ImGui::PopStyleColor();
	ImGui::Separator();

	const char* dataStructures[] = { "Red Black Tree", "Heap" };
	ImGui::Text("Algorithm");
	ImGui::Combo("##Algorithm", &selectedDataStructure, dataStructures, IM_ARRAYSIZE(dataStructures));

	ImGui::Separator();
	
	ImGui::Text("Min/Max items to find");
	ImGui::SliderInt("##Min/Max items to find", &itemsToFind, 0, 200);

	if (ImGui::Button("Search..."))
	{

	}

	const int columnCountSearchResults = 15;
	const char* columnNamesSearchResults[] = { "Station Code", "State", "Normal Average Year (in)", 
		                                       "January", "February", "March", 
		                                       "April", "May", "June", "July", 
		                                       "August", "September", "October",
		                                       "November", "December"};
	ImGui::Text("Search Results");
	if (ImGui::BeginTable("Search Results", 15, flags))
	{
		for (size_t i = 0; i < columnCountSearchResults; i++)
		{
			ImGui::TableNextColumn();
			ImGui::PushID(i);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("%s", columnNamesSearchResults[i]);
			ImGui::Spacing();

			ImGui::PopID();
		}

		// Display search results
		for (size_t i = 0; i < precipitationSearchResults.size(); i++)
		{
			const PrecipitationNormal precipitationNormal = precipitationSearchResults[i];

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			if (ImGui::Button(precipitationNormal.stationCode.c_str()))
			{
				SearchStation(precipitationNormal.stationCode.c_str());
			}
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(stations.Search(precipitationNormal.stationCode).state.c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%.2f", precipitationNormal.normalAverageForYear);
			
			// Display each months precipitation normal
			for (size_t j = 0; j < 12; j++)
			{
				ImGui::TableSetColumnIndex(3 + j);
				ImGui::Text("%.2f", precipitationNormal.normalAverageByMonth[j]);
			}
		}

		ImGui::EndTable();
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::SearchStation(const std::string& stationCode)
{
	intmax_t index = stations.Find(stationCode);
	if (index != -1)
	{
		stationSearchResults.emplace_back(stations.Search(index));
		recentStationSearchIndex = stationSearchResults.size() - 1;
	}

	std::memset(stationCodeBuffer, 0, 12);
}