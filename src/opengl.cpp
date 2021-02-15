#include "opengl.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

//////////////////////////////////////////////////////////////////////////////
// Vertex Buffer
//////////////////////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &bufferID);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
}

void VertexBuffer::unBind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////////
// Vertex Array
//////////////////////////////////////////////////////////////////////////////
VertexArray::VertexArray() {
	glGenVertexArrays(1, &arrayID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &arrayID);
}

void VertexArray::addBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout) {
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * VertexBufferLayoutElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(arrayID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////
// Index Buffer
//////////////////////////////////////////////////////////////////////////////
IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count) : count(count) {
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &bufferID);
}

void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////////
// Shader
//////////////////////////////////////////////////////////////////////////////
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: rendererID(0)
{
	std::string vertex = getShaderSrc(vertexPath);
	std::string fragment = getShaderSrc(fragmentPath);
	rendererID = createShaderProgram(vertex, fragment);
}

Shader::~Shader() {
	glDeleteProgram(rendererID);
}

void Shader::bind() const {
	glUseProgram(rendererID);
}

void Shader::unbind() const {
	glUseProgram(0);
}

void Shader::setUniform1i(const std::string & name, int value) {
	bind();
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform2i(const std::string & name, int v0, int v1) {
	bind();
	glUniform2i(getUniformLocation(name), v0, v1);
}

void Shader::setUniform1f(const std::string & name, float value)
{
	bind();
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform2f(const std::string & name, float v0, float v1) {
	bind();
	glUniform2f(getUniformLocation(name), v0, v1);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
	bind();
	glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3) {
	bind();
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMat4(const std::string & name, float* m) {
	bind();
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, static_cast<GLfloat*>(m));
}

GLint Shader::getUniformLocation(const std::string & name) {
	if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
		return uniformLocationCache[name];
	}
	unsigned int location = glGetUniformLocation(rendererID, name.c_str());
	if (location < 0) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}
	uniformLocationCache[name] = location;
	return location;
}

GLuint Shader::compileShader(GLenum type, const std::string& source) {
	GLuint id = glCreateShader(type);
	const char * src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Handle errors if the shader compilation failed
	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char * message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Shader compilation failed:\n" << message << std::endl;
		}
	}

	return id;
}

std::string Shader::getShaderSrc(const std::string& filePath) {
	std::string shaderSrc;
    std::ifstream fileSrc;
    fileSrc.open(filePath);
    if (fileSrc.is_open()) {
        std::string line;
        while (std::getline(fileSrc, line)) shaderSrc += "\n" + line;
        fileSrc.close();
    } else {
        std::cout << "Could not read shader: " << filePath << std::endl;
    }
	return shaderSrc;
}

GLuint Shader::createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) {

	GLuint program = glCreateProgram();
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// TODO: assert that shaders were compiled successfully
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			GLchar * message = (GLchar*)alloca(length * sizeof(GLchar));
			glGetProgramInfoLog(program, length, &length, message);
			std::cout << "Program link failed:\n" << message << std::endl;
		}
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glValidateProgram(program);
	
	// TODO: Error handling

	// Delete the shaders once the program has been created
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;

}

//////////////////////////////////////////////////////////////////////////////
// Texture
//////////////////////////////////////////////////////////////////////////////
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(int width, int height) : width(width), height(height)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture::Texture(const std::string & path)
	: textureID(0), filePath(path), localBuffer(nullptr),
	width(0), height(0), bitsPerPixel(0)
{
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 4);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localBuffer) {
		stbi_image_free(localBuffer);
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}

void Texture::bind(unsigned int slot /*= 0*/) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetData(void* data, uint32_t size, GLenum format)
{
	// TODO: THIS FORMAT THING IS WRONG I THINK
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}