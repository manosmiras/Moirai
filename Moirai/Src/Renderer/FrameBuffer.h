#pragma once
#include <glad/glad.h>

class FrameBuffer
{
public:
	FrameBuffer(GLsizei width, GLsizei height);
	~FrameBuffer();
	void Bind();
	void Unbind();
	void Rescale(GLsizei width, GLsizei height);
	GLuint textureId;
	GLsizei width;
	GLsizei height;
private:
	GLuint fbo, rbo;
};
