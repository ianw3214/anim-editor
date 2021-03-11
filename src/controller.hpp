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
	static void LoadFreshAnimDataFromImage(const std::string& path, const std::string& file);
	static const std::string& GetCurrentSpritesheetPath();
private:
	static AnimData sAnimData;
};