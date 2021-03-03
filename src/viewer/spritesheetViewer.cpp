#include "spritesheetViewer.hpp"

#include "graphics/opengl.hpp"
#include "graphics/renderer.hpp"

void SpritesheetViewer::UpdateSprite(const std::string& path)
{
	m_texture = std::make_unique<Texture>(path);
	m_width = m_texture->getWidth();
	m_height = m_texture->getHeight();
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
	if (m_texture)
	{
		// Renderer::DrawQuad(x_offset, y_offset, width, height, texture);
		Renderer::DrawQuad(m_xOffset, m_yOffset, m_width, m_height, m_texture.get());
	}
}