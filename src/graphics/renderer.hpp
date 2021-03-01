#pragma once

#include "opengl.hpp"

struct Colour
{
    float r;
    float g;
    float b;
};

class Renderer
{
public:
    static void Init();
    static void Clear(float r=0.f, float g=0.f, float b=0.f);
    static void Clear(const Colour& colour);
    static void UpdateWindowSize(float w, float h);

    static void DrawTiledQuad(float x, float y, float w, float h);
    static void DrawLine(float x1, float y1, float x2, float y2, const Colour& colour);
    static void DrawLineStrip(float * data, int num_vertices, const Colour& colour);
    static void DrawQuad(float x, float y, float w, float h, const Colour& colour, float alpha);
    static void DrawQuad(float x, float y, float w, float h, Texture* texture);
private:
    static Shader * basic_shader;
    static Shader * texture_shader;
    static Shader * tile_shader;
};