#version 330 core

in vec3 vertColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D _texture;
//uniform vec4 myColor;

void main()
{
	color = texture(_texture, texCoord) * vec4(vertColor, 1.0);
	//color = vec4(vertColor, 1.0);
}