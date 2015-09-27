#ifndef SHADER_H
#define SHADER_H

#define GLEW_STATIC

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "FileUtil.h"

class Shader
{
public:
	Shader(const char* vertShader, const char* fragShader);
	~Shader();
	void Use();
	void SetUniform(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2);
	void SetUniform(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	void SetUniform(const char* uniformName, const glm::mat4& mat);

private:
	GLuint program;

	static GLuint CreateShader(const char* filePath, GLenum shaderType);
};

#endif