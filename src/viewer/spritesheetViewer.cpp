#include "spritesheetViewer.hpp"

#include "graphics/opengl.hpp"
#include "graphics/renderer.hpp"

#include "controller.hpp"

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

	if (m_texture)
	{
		Renderer::DrawQuad(m_xOffset, m_yOffset, m_width, m_height, m_texture.get());
	}
}