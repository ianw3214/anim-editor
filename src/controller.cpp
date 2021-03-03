#include "controller.hpp"

AnimData Controller::sAnimData;

void Controller::LoadFreshAnimDataFromImage(const std::string& path, const std::string& file)
{
	sAnimData.m_currSpritesheetPath = path;
	sAnimData.m_currSpritesheetFilename = file;
	sAnimData.m_fps = AnimData::kDefaultFps;
	sAnimData.m_frameWidth = AnimData::kDefaultFrameWidth;
	sAnimData.m_frameHeight = AnimData::kDefaultFrameHeight;
}

const std::string& Controller::GetCurrentSpritesheetPath()
{
	return sAnimData.m_currSpritesheetPath;
}