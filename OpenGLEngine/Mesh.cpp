#include "Mesh.h"

// TODO: Check if shader attributes can be set outside of vbo/texture binds

// Creates an empty mesh object
Mesh::Mesh()
{
	this->numVerts = 0;
	this->numIndices = 0;
	this->numTexCoords = 0;
	this->numColors = 0;

	glGenVertexArrays(1, &vao);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vboVerts);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vboTexCoords);
	glDeleteBuffers(1, &vboColors);
}

void Mesh::SetVertices(GLfloat verts[], int numVerts)
{
	this->numVerts = numVerts;

	glGenBuffers(1, &vboVerts);

	glBindVertexArray(vao); // Bind vao
	glBindBuffer(GL_ARRAY_BUFFER, vboVerts); // Bind vbo

	glBufferData(GL_ARRAY_BUFFER, numVerts * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW); // Set the vertices that vbo should use

	// Shader attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Configure vertex attribute
	glEnableVertexAttribArray(0); // Enable vertex attribute 0 to be used by vertex shader

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
	glBindVertexArray(0); // Unbind vao
}

void Mesh::SetIndices(GLuint indices[], int numIndices)
{
	this->numIndices = numIndices;

	glGenBuffers(1, &ebo);

	glBindVertexArray(vao); // Bind vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // Bind ebo

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW); // Set indices

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind ebo
	glBindVertexArray(0); // Unbind vao
}

void Mesh::SetNormals(GLfloat normals[], int numNormals)
{
	this->numNormals = numNormals;

	glGenBuffers(1, &vboNormals);

	glBindVertexArray(vao); // Bind vao
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals); // Bind vbo

	glBufferData(GL_ARRAY_BUFFER, numNormals * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the normals that vbo should use

	// Shader attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Configure vertex attribute
	glEnableVertexAttribArray(1); // Enable vertex attribute 1 to be used by vertex shader

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
	glBindVertexArray(0); // Unbind vao
}

void Mesh::SetTexCoords(GLfloat texCoords[], int numTexCoords)
{
	this->numTexCoords = numTexCoords;

	glGenBuffers(1, &vboTexCoords);

	glBindVertexArray(vao); // Bind vao
	glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords); // Bind vbo

	glBufferData(GL_ARRAY_BUFFER, numTexCoords * 2 * sizeof(GLfloat), texCoords, GL_STATIC_DRAW); // Set tex coords

	// Shader attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // Configure tex coords attribute
	glEnableVertexAttribArray(2); // Enable vertex attribute 2 to be used by vertex shader

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
	glBindVertexArray(0); // Unbind vao
}

void Mesh::SetTexture(const char* filePath)
{
	Image img(filePath);

	glGenTextures(1, &texture);

	glBindVertexArray(vao); // Bind vao
	glActiveTexture(GL_TEXTURE0); // Activate texture
	glBindTexture(GL_TEXTURE_2D, texture); // Bind texture

	// Might need to use glCompressedTexImage2D for dds loading
	if (img.numChannels == 3) // RGB
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
	else if (img.numChannels == 4) // RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
	glBindVertexArray(0); // Unbind vao
}

void Mesh::SetVertexColors(GLfloat colors[], int numColors)
{
	this->numColors = numColors;

	glGenBuffers(1, &vboColors);

	glBindVertexArray(vao); // Bind vao
	glBindBuffer(GL_ARRAY_BUFFER, vboColors); // Bind vbo

	glBufferData(GL_ARRAY_BUFFER, numColors * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the vertices that vbo should use

	// Shader attributes
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0); // Configure vertex color attribute
	glEnableVertexAttribArray(3); // Enable vertex attribute 3 to be used by vertex shader

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
	glBindVertexArray(0); // Unbind vao
}

void Mesh::Draw()
{
	Draw(GL_TRIANGLES);
}

void Mesh::Draw(GLenum mode)
{
	glBindVertexArray(vao); // Bind vao so it can be drawn
	glActiveTexture(GL_TEXTURE0); // Activate the first texture
	glBindTexture(GL_TEXTURE_2D, texture); // Bind texture

	if (numIndices > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // Bind ebo so it can be drawn
		glDrawElements(mode, numIndices, GL_UNSIGNED_INT, 0); // Draw using vertices and indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind ebo
	}
	else
		glDrawArrays(mode, 0, numVerts); // Draw using vertices

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
	glBindVertexArray(0); // Unbind vao after it has finished drawing
}