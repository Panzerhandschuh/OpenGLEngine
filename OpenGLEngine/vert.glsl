#version 330 core

layout (location = 0) in vec3 _pos;
layout (location = 1) in vec3 _vertColor;
layout (location = 2) in vec2 _texCoord;

out vec3 vertColor;
out vec2 texCoord;

//uniform vec3 offset;

void main()
{
	gl_Position = vec4(_pos, 1.0);
	vertColor = _vertColor;
	texCoord = _texCoord;
}