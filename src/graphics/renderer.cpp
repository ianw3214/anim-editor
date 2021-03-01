#include "renderer.hpp"

#include <gl/glew.h>

Shader * Renderer::basic_shader = nullptr;
Shader * Renderer::texture_shader = nullptr;
Shader * Renderer::tile_shader = nullptr;

const unsigned int SQUARE_INDICES[6] = { 0, 1, 3, 0, 2, 3 };
const unsigned int LINE_INDICES[2] = { 0, 1 };

void Renderer::Init()
{
    basic_shader = new Shader(Shader::DefaultShader::BASIC);
    texture_shader = new Shader(Shader::DefaultShader::TEXTURE);
	tile_shader = new Shader(Shader::DefaultShader::TILE);

	
	UpdateWindowSize(1280.f, 720.f);
}

void Renderer::Clear(float r, float g, float b)
{
    glClearColor(1.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Clear(const Colour& colour)
{
    Clear(colour.r, colour.g, colour.b);
}

void Renderer::UpdateWindowSize(float w, float h)
{
	basic_shader->setUniform1f("u_screenWidth", w);
	basic_shader->setUniform1f("u_screenHeight", h);
	texture_shader->setUniform1f("u_screenWidth", w);
	texture_shader->setUniform1f("u_screenHeight", h);
	tile_shader->setUniform1f("u_screenWidth", w);
	tile_shader->setUniform1f("u_screenHeight", h);
}

void Renderer::DrawTiledQuad(float x, float y, float w, float h)
{
	// TODO: Store buffers somewhere and adjust unfiforms to draw
	float positions[8] = {
		x, y,
		x, y + h,
		x + w, y,
		x + w, y + h
	};
	VertexArray		va;
	VertexBuffer	vb(positions, sizeof(float) * 8);
	IndexBuffer		ib(SQUARE_INDICES, 6);
	// Specify the layout of the buffer data
	VertexBufferLayout layout;
	layout.pushFloat(2);
	va.addBuffer(vb, layout);

	// Issue the actual draw call
	tile_shader->bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, const Colour& colour)
{
    float positions[4] = { x1, y1, x2, y2 };
	VertexArray		va;
	VertexBuffer	vb(positions, sizeof(float) * 4);
	IndexBuffer		ib(LINE_INDICES, 2);
	// Specify the layout of the buffer data
	VertexBufferLayout layout;
	layout.pushFloat(2);
	va.addBuffer(vb, layout);

	// Set the uniform to draw the right colour
	basic_shader->setUniform4f("u_Colour", 1.f, 1.f, 1.f, 1.f);
	basic_shader->bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_LINES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLineStrip(float * data, int num_vertices, const Colour& colour)
{
	unsigned int * indices = new unsigned int[num_vertices];
	for (unsigned int i = 0; i < (unsigned int) num_vertices; ++i) indices[i] = i;

	float * positions = data;
	VertexArray va;
	VertexBuffer vb(positions, sizeof(float) * num_vertices * 2);
	IndexBuffer		ib(indices, num_vertices);
	// Specify the layout of the buffer data
	VertexBufferLayout layout;
	layout.pushFloat(2);
	va.addBuffer(vb, layout);

	// Set the uniform to draw the right colour
	basic_shader->setUniform4f("u_Colour", colour.r, colour.g, colour.b, 1.f);
	basic_shader->bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_LINE_STRIP, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawQuad(float x, float y, float w, float h, const Colour& colour, float alpha)
{
    // TODO: Store buffers somewhere and adjust unfiforms to draw
    float positions[8] = {
        x, y,
        x, y + h,
        x + w, y,
        x + w, y + h
    };
	VertexArray		va;
	VertexBuffer	vb(positions, sizeof(float) * 8);
	IndexBuffer		ib(SQUARE_INDICES, 6);
	// Specify the layout of the buffer data
	VertexBufferLayout layout;
	layout.pushFloat(2);
	va.addBuffer(vb, layout);

	// Issue the actual draw call
    basic_shader->setUniform4f("u_Colour", colour.r, colour.g, colour.b, alpha);
	basic_shader->bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawQuad(float x, float y, float w, float h, Texture* texture)
{
	float positions[16] = {
		x, y, 0.f, 0.f,
		x, y + h, 0.f, 1.f,
		x + w, y, 1.f, 0.f,
		x + w, y + h, 1.f, 1.f
	};
	VertexArray		va;
	VertexBuffer	vb(positions, sizeof(float) * 16);
	IndexBuffer		ib(SQUARE_INDICES, 6);
	// Specify the layout of the buffer data
	VertexBufferLayout layout;
	layout.pushFloat(2);
	layout.pushFloat(2);
	va.addBuffer(vb, layout);

	// Bind the texture and draw
	texture->bind();
	texture_shader->bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}