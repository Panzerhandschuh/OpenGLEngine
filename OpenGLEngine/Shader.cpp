#include "Shader.h"

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
	//GLfloat time = glfwGetTime();
	//GLfloat green = (sin(time) / 2.0f) + 0.5f;
	//glUniform4f(0, 0.0f, green, 0.0f, 1.0f);
	glUseProgram(program);
}

void Shader::SetAttribute(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLint location = glGetUniformLocation(program, uniformName);
	glUniform3f(location, v0, v1, v2);
}

void Shader::SetAttribute(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLint location = glGetUniformLocation(program, uniformName);
	glUniform4f(location, v0, v1, v2, v3);
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