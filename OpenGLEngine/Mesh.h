#ifndef MESH_H
#define MESH_H

#define GLEW_STATIC

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Image.h"

class Mesh
{
public:
	GLenum drawMode = GL_TRIANGLES;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> colors;
	std::vector<GLuint> indices;

	Mesh();
	~Mesh();
	void SetVertices(std::vector<glm::vec3>& verts);
	void SetNormals(std::vector<glm::vec3>& normals);
	void SetUvs(std::vector<glm::vec2>& uvs);
	void SetVertexColors(std::vector<glm::vec3>& colors);
	void SetIndices(std::vector<GLuint>& indices);
	void SetTexture(const char* imgFile);
	void Draw();

private:
	GLuint vao, ebo, vboVerts, vboNormals, vboUvs, vboColors, texture;
};

#endif