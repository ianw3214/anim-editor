#include "controller.hpp"

AnimData Controller::sAnimData;

void Controller::LoadImage(const std::string& path, const std::string& file)
{
	sAnimData.m_currSpritesheetPath = path;
	sAnimData.m_currSpritesheetFilename = file;
}

void Controller::LoadFreshAnimDataFromImage(const std::string& path, const std::string& file)
{
	sAnimData.m_currSpritesheetPath = path;
	sAnimData.m_currSpritesheetFilename = file;
	sAnimData.m_fps = AnimData::kDefaultFps;
	sAnimData.m_frameWidth = AnimData::kDefaultFrameWidth;
	sAnimData.m_frameHeight = AnimData::kDefaultFrameHeight;

	sAnimData.m_states.push_back({"State1", 0, 2});
}

const std::string& Controller::GetCurrentSpritesheetPath()
{
	return sAnimData.m_currSpritesheetPath;
}

uint16_t Controller::GetFrameWidth()
{
	return sAnimData.m_frameWidth;
}

uint16_t Controller::GetFrameHeight()
{
	return sAnimData.m_frameHeight;
}

const std::vector<AnimData::State>& Controller::GetStates()
{
	return sAnimData.m_states;
}

void Controller::SetFrameWidth(uint16_t width)
{
	sAnimData.m_frameWidth = width;
}

void Controller::SetFrameHeight(uint16_t height)
{
	sAnimData.m_frameHeight = height;
}

void Controller::AddNewState(uint16_t start, uint16_t end, std::string name)
{
	sAnimData.m_states.push_back({ name, start, end });
}

uint16_t* Controller::GetFrameWidthAddress()
{
	return &(sAnimData.m_frameWidth);
}

uint16_t* Controller::GetFrameHeightAddress()
{
	return &(sAnimData.m_frameHeight);
}