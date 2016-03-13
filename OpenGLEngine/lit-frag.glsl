#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;
//in vec3 vertColor;

out vec4 color;

uniform sampler2D _texture;
uniform vec3 objectColor;
uniform vec3 ambient;
uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 viewPos;

void main()
{
	// Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDirNorm = normalize(lightDir);
	float diff = max(dot(norm, normalize(lightDirNorm)), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular
	float specStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDirNorm, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	//color = texture(_texture, texCoord) * vec4(result, 1.0);
	color = vec4(result, 1.0);
}