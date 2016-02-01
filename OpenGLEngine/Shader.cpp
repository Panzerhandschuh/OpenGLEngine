#include "Shader.h"

using namespace glm;

Shader::Shader(const char* vertShaderFile, const char* fragShaderFile)
{
	// Create shaders
	GLuint vertShader = CreateShader(vertShaderFile, GL_VERTEX_SHADER);
	GLuint fragShader = CreateShader(fragShaderFile, GL_FRAGMENT_SHADER);

	// Create shader program
	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	// Check for linking errors
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		char* infoLog = new char[logLength];
		glGetProgramInfoLog(program, logLength, 0, infoLog);
		std::cerr << "Error: Could not link shaders\n" << infoLog << std::endl;
		delete[] infoLog;
	}

	// Clean up shaders
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::Use()
{
	glUseProgram(program);
}

void Shader::SetUniform(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLint loc = glGetUniformLocation(program, uniformName);
	glUniform3f(loc, v0, v1, v2);
}

void Shader::SetUniform(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLint loc = glGetUniformLocation(program, uniformName);
	glUniform4f(loc, v0, v1, v2, v3);
}

void Shader::SetUniform(const char* uniformName, const glm::vec3& vec)
{
	GLint loc = glGetUniformLocation(program, uniformName);
	glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::SetUniform(const char* uniformName, const glm::mat4& mat)
{
	GLuint loc = glGetUniformLocation(program, uniformName);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

GLuint Shader::CreateShader(const char* filePath, GLenum shaderType)
{
	// Load and compile shader
	GLuint shader = glCreateShader(shaderType);
	std::string shaderStr = FileUtil::ReadFile(filePath);
	const char* shaderSrc = shaderStr.c_str();
	glShaderSource(shader, 1, &shaderSrc, 0); // Load shader from source file
	glCompileShader(shader);

	// Check for shader compile errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		char* infoLog = new char[logLength];
		glGetShaderInfoLog(shader, logLength, 0, infoLog);
		std::cerr << "Error: Could not compile shader\n" << infoLog << std::endl;
		delete[] infoLog;
	}

	return shader;
}