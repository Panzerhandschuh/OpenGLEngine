#version 330 core

in vec2 texCoord;
//in vec3 vertColor;

out vec4 color;

//uniform sampler2D _texture;
uniform vec3 objectColor;

void main()
{
	//color = texture(_texture, texCoord)/* * vec4(vertColor, 1.0)*/;
	color = vec4(objectColor, 1.0);
}