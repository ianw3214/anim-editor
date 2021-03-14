#include "spritesheetViewer.hpp"

#include "graphics/opengl.hpp"
#include "graphics/renderer.hpp"

#include "controller.hpp"

#include <SDL2/SDL.h>

SpritesheetViewer::SpritesheetViewer()
    : m_currSpritesheetPath()
    , m_texture()
    , m_width()
    , m_height()
    , m_xOffset(0.0)
    , m_yOffset(0.0)
    , m_panning(false)
{
}

void SpritesheetViewer::UpdateSprite(const std::string& path)
{
	m_texture = std::make_unique<Texture>(path);
	m_width = static_cast<float>(m_texture->getWidth());
	m_height = static_cast<float>(m_texture->getHeight());
}

bool SpritesheetViewer::HandleEvent(const SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_SPACE)
        {
            m_panning = true;
            return true;
        }
    }
    if (e.type == SDL_KEYUP)
    {
        if (e.key.keysym.sym == SDLK_SPACE)
        {
            m_panning = false;
            return true;
        }
    }
    if (e.type == SDL_MOUSEMOTION)
    {
        if (m_panning)
        {
            m_xOffset+= static_cast<float>(e.motion.xrel);
            m_yOffset += static_cast<float>(e.motion.yrel);
            return true;
        }
    }
    return false;
}

void SpritesheetViewer::Render()
{
    // Check to see if the sprite sheet has been changed
    const std::string& newpath = Controller::GetCurrentSpritesheetPath();
    if (m_currSpritesheetPath != newpath)
    {
        m_currSpritesheetPath = newpath;
        UpdateSprite(newpath);
    }

    // Draw the actual spritesheet texture
	if (m_texture)
	{
		Renderer::DrawQuad(m_xOffset, m_yOffset, m_width, m_height, m_texture.get());
	}

    // Highlight the spritesheet frame that the mouse is hovering over
    if (m_texture)
    {
        // TODO: Negative positions are weird
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        float x1 = ((mouse_x - (int)m_xOffset) / Controller::GetFrameWidth()) * Controller::GetFrameWidth() + m_xOffset;
        float y1 = ((mouse_y - (int)m_yOffset) / Controller::GetFrameHeight()) * Controller::GetFrameHeight() + m_yOffset;
        float x2 = ((mouse_x - (int)m_xOffset) / Controller::GetFrameWidth() + 1) * Controller::GetFrameWidth() + m_xOffset;
        float y2 = ((mouse_y - (int)m_yOffset) / Controller::GetFrameHeight() + 1) * Controller::GetFrameHeight() + m_yOffset;
        float lines[10] = { x1, y1, x1, y2, x2, y2, x2, y1, x1, y1 };
        Renderer::DrawLineStrip(lines, 5, Colour{ 255.f, 255.f, 255.f });
    }
}