#ifndef SHADER_H
#define SHADER_H

#define GLEW_STATIC

#include <GL\glew.h>
#include "FileUtil.h"

class Shader
{
public:
	Shader(const char* vertShader, const char* fragShader);
	~Shader();
	void Use();
	void SetAttribute(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2);
	void SetAttribute(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

private:
	GLuint program;

	static GLuint CreateShader(const char* filePath, GLenum shaderType);
};

#endif