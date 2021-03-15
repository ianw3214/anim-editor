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
    , m_scale(1.0)
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
    if (e.type == SDL_MOUSEWHEEL)
    {
        // scroll up
        if (e.wheel.y > 0)
        {
            m_scale *= 1.2f;
        }
        // scroll down
        if (e.wheel.y < 0)
        {
            m_scale /= 1.2f;
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
		Renderer::DrawQuad(m_xOffset, m_yOffset, m_width * m_scale, m_height * m_scale, m_texture.get());
	}

    // Highlight the spritesheet frame that the mouse is hovering over
    if (m_texture)
    {
        const int width = static_cast<int>(Controller::GetFrameWidth() * m_scale);
        const int height = static_cast<int>(Controller::GetFrameHeight() * m_scale);
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        float x1 = ((mouse_x - (int)m_xOffset) / width) * width + m_xOffset;
        float y1 = ((mouse_y - (int)m_yOffset) / height) * height + m_yOffset;
        float x2 = ((mouse_x - (int)m_xOffset) / width + 1) * width + m_xOffset;
        float y2 = ((mouse_y - (int)m_yOffset) / height + 1) * height + m_yOffset;
        float lines[10] = { x1, y1, x1, y2, x2, y2, x2, y1, x1, y1 };
        Renderer::DrawLineStrip(lines, 5, Colour{ 255.f, 255.f, 255.f });
    }
}