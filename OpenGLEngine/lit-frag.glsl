#version 330 core

//in vec3 vertColor;
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 color;

uniform sampler2D _texture;
uniform vec3 objectColor;
uniform vec3 ambient;
uniform vec3 lightColor;
uniform vec3 lightDir;

void main()
{
	// Diffuse
	float diff = max(dot(normal, normalize(lightDir)), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	color = texture(_texture, texCoord) * vec4(result, 1.0);
}