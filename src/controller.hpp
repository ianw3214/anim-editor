#pragma once

#include <string>
#include <vector>
#include <unordered_map>

///////////////////////////////////////////////////////////////////////////////
// Representation of animation data in code
///////////////////////////////////////////////////////////////////////////////
class AnimData
{
public:
	const static int kDefaultFps = 24;
	const static int kDefaultFrameWidth = 16;
	const static int kDefaultFrameHeight = 16;
public:
	struct State {
		std::string m_name;
		uint16_t m_start;
		uint16_t m_end;
		std::unordered_map<std::string, State*> m_transitions;
	};

	std::string m_currSpritesheetPath;
	std::string m_currSpritesheetFilename;
	uint16_t m_fps;
	uint16_t m_frameWidth;
	uint16_t m_frameHeight;
	std::vector<State> m_states;
};

///////////////////////////////////////////////////////////////////////////////
// The controller class grants global access to anim data of current project
///////////////////////////////////////////////////////////////////////////////
class Controller
{
public:
	static void LoadImage(const std::string& path, const std::string& file);
	static void LoadFreshAnimDataFromImage(const std::string& path, const std::string& file);
	static const std::string& GetCurrentSpritesheetPath();

	static uint16_t GetFrameWidth();
	static uint16_t GetFrameHeight();
	static const std::vector<AnimData::State>& GetStates();

	static void SetFrameWidth(uint16_t width);
	static void SetFrameHeight(uint16_t height);
	static void AddNewState(uint16_t start, uint16_t end, std::string name = "");

	// direct access to anim data properties for imgui
	static uint16_t* GetFrameWidthAddress();
	static uint16_t* GetFrameHeightAddress();
private:
	static AnimData sAnimData;
};