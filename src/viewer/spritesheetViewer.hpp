#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <memory>

class Texture;
class SpritesheetViewer
{
public:
	SpritesheetViewer() = default;

	void UpdateSprite(const std::string& path);

	bool HandleEvent(const SDL_Event& e);
	void Render();
private:
	std::string m_currSpritesheetPath;
	std::unique_ptr<Texture> m_texture;
	float m_width;
	float m_height;

	// 'Camera' information
	float m_xOffset;
	float m_yOffset;
	bool m_panning;
};