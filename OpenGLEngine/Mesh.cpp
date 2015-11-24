#include "Mesh.h"

// Creates an empty mesh object
Mesh::Mesh()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vboVerts);
	glGenBuffers(1, &vboNormals);
	glGenBuffers(1, &vboUvs);
	glGenBuffers(1, &vboColors);
	glGenBuffers(1, &ebo);

	// Configure shader attributes
	glBindVertexArray(vao); // Bind vao

	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboVerts); // Bind vbo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Configure vertex attribute
	glEnableVertexAttribArray(0); // Enable vertex attribute 0 to be used by vertex shader

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals); // Bind vbo
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Configure normal attribute
	glEnableVertexAttribArray(1); // Enable vertex attribute 1 to be used by vertex shader

	// Uvs
	glBindBuffer(GL_ARRAY_BUFFER, vboUvs); // Bind vbo
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // Configure uv attribute
	glEnableVertexAttribArray(2); // Enable vertex attribute 2 to be used by vertex shader

	// Colors
	glBindBuffer(GL_ARRAY_BUFFER, vboColors); // Bind vbo
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0); // Configure vertex color attribute
	glEnableVertexAttribArray(3); // Enable vertex attribute 3 to be used by vertex shader
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
	glBindVertexArray(0); // Unbind vao
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vboVerts);
	glDeleteBuffers(1, &vboNormals);
	glDeleteBuffers(1, &vboUvs);
	glDeleteBuffers(1, &vboColors);
	glDeleteBuffers(1, &ebo);
}

void Mesh::SetVertices(vector<glm::vec3>& verts)
{
	this->vertices = verts;

	glBindBuffer(GL_ARRAY_BUFFER, vboVerts); // Bind vbo
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW); // Set the vertices that vbo should use
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
}

void Mesh::SetNormals(vector<glm::vec3>& normals)
{
	this->normals = normals;

	glBindBuffer(GL_ARRAY_BUFFER, vboNormals); // Bind vbo
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW); // Set the normals that vbo should use
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
}

void Mesh::SetUvs(vector<glm::vec2>& uvs)
{
	this->uvs = uvs;

	glBindBuffer(GL_ARRAY_BUFFER, vboUvs); // Bind vbo
	glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW); // Set the uvs that vbo should use
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
}

void Mesh::SetVertexColors(vector<glm::vec3>& colors)
{
	this->colors = colors;

	glBindBuffer(GL_ARRAY_BUFFER, vboColors); // Bind vbo
	glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(glm::vec3), &this->colors[0], GL_STATIC_DRAW); // Set the colors that vbo should use
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
}

void Mesh::SetIndices(vector<GLuint>& indices)
{
	this->indices = indices;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // Bind ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW); // Set the indices that ebo should use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind ebo
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

void Mesh::Draw()
{
	glActiveTexture(GL_TEXTURE0); // Activate the first texture
	glBindTexture(GL_TEXTURE_2D, texture); // Bind texture

	glBindVertexArray(vao); // Bind vao so it can be drawn
	if (indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // Bind ebo so it can be drawn
		glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0); // Draw using vertices and indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind ebo
	}
	else
		glDrawArrays(drawMode, 0, vertices.size()); // Draw using vertices
	glBindVertexArray(0); // Unbind vao after it has finished drawing

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
}