#pragma once

const char * basic_vertex = 
"#version 330 core\n"
"layout(location = 0) in vec2 position;"
"layout(location = 1) in vec2 texCoord;"
"uniform float u_screenWidth;"
"uniform float u_screenHeight;"
"out vec4 vScreenPos;"
"out vec2 vTexCoord;"
"void main() {"
"    float x = mix(-1.0,  1.0, position.x / u_screenWidth);"
"    float y = mix( 1.0, -1.0, position.y / u_screenHeight);"
"    gl_Position = vec4(x, y, 0.0, 1.0);"
"    vScreenPos = vec4(x, y, 0.0, 1.0);"
// Invert y because y axis is positive in the downards direction
"    vTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);"
"}"
;

const char * basic_fragment =
"#version 330 core\n"
"out vec4 colour;"
"uniform vec4 u_Colour;"
"void main() {"
"    colour = u_Colour;"
"}"
;

const char * basic_texture =
"#version 330 core\n"
"in vec2 vTexCoord;"
"out vec4 colour;"
"uniform sampler2D u_Texture;"
"void main() {"
"	vec4 texColour = texture(u_Texture, vTexCoord);"
"	colour = texColour;"
"}"
;

const char* tile_texture =
"#version 330 core\n"
"in vec4 vScreenPos;"
"out vec4 colour;"
"void main() {"
"    float x_key = floor(mod(vScreenPos.x * 10.0, 2.0));"
"    float y_key = floor(mod(vScreenPos.y * 8.0, 2.0));"
"    colour = mod(x_key + y_key, 2.0) == 0 ? vec4(0.2, 0.2, 0.2, 1.0) : vec4(0.1, 0.1, 0.1, 1.0);"
"}"
;