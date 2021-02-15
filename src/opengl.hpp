#pragma once

#include <GL/glew.h>

#include <vector>
#include <string>
#include <unordered_map>


////////////////////////////////////////////////////////////////////
class VertexBuffer {
////////////////////////////////////////////////////////////////////
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unBind() const;
private:
	// This represents the vertex buffer id in openGL
	GLuint bufferID;
};

////////////////////////////////////////////////////////////////////
struct VertexBufferLayoutElement {
////////////////////////////////////////////////////////////////////
	GLenum type;
	unsigned int count;
	GLboolean normalized;

	static unsigned int getSizeOfType(GLenum type) {
		switch (type) {
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
		}
		// Notify any unsupported types
		// ASSERT(false);
		return 0;
	}
};

////////////////////////////////////////////////////////////////////
class VertexBufferLayout {
////////////////////////////////////////////////////////////////////
public:

	VertexBufferLayout() : stride(0) {}

	// Getter methods
	inline const std::vector<VertexBufferLayoutElement> getElements() const { return elements; }
	inline unsigned int getStride() const { return stride; }

	// Functions to add attributes to the layout
	void pushFloat(unsigned int count) {
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_FLOAT) * count;
	}

	void pushUint(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	void pushUchar(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += VertexBufferLayoutElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

private:
	std::vector<VertexBufferLayoutElement> elements;
	unsigned int stride;

};

////////////////////////////////////////////////////////////////////
class VertexArray {
////////////////////////////////////////////////////////////////////
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer & vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
private:
	GLuint arrayID;
};

////////////////////////////////////////////////////////////////////
class IndexBuffer {
////////////////////////////////////////////////////////////////////
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return count; }
private:
	// This represents the index buffer id in openGL
	GLuint bufferID;
	unsigned int count;
};

////////////////////////////////////////////////////////////////////
class Shader {
////////////////////////////////////////////////////////////////////
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void bind() const;
	void unbind() const;

	// Set uniforms
	void setUniform1i(const std::string& name, int value);
	void setUniform2i(const std::string & name, int v0, int v1);
	void setUniform1f(const std::string & name, float value);
	void setUniform2f(const std::string & name, float v0, float v1);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4(const std::string& name, float* m);
private:
	std::string vertexSource, fragmentSource;
	GLuint rendererID;

	// caching for uniforms
	std::unordered_map<std::string, GLint> uniformLocationCache;

	GLint getUniformLocation(const std::string& name);
	// Shader compilation functions
	GLuint compileShader(GLenum type, const std::string& source);
	std::string getShaderSrc(const std::string& filePath);
	GLuint createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
};

////////////////////////////////////////////////////////////////////
class Texture {
////////////////////////////////////////////////////////////////////
public:
    Texture(int width, int height);
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    void SetData(void* data, uint32_t size, GLenum format = GL_RGBA);

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
private:
    GLuint textureID;
    std::string filePath;
    unsigned char * localBuffer;
    int width, height, bitsPerPixel;
};