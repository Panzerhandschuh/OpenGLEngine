#ifndef MESH_H
#define MESH_H

#define GLEW_STATIC

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Image.h"

using namespace std;

class Mesh
{
public:
	GLenum drawMode = GL_TRIANGLES;
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> uvs;
	vector<glm::vec3> colors;
	vector<GLuint> indices;

	Mesh();
	~Mesh();
	void SetVertices(vector<glm::vec3> verts);
	void SetNormals(vector<glm::vec3> normals);
	void SetUvs(vector<glm::vec2> uvs);
	void SetVertexColors(vector<glm::vec3> colors);
	void SetIndices(vector<GLuint> indices);
	void SetTexture(const char* imgFile);
	void Draw();

private:
	GLuint vao, ebo, vboVerts, vboNormals, vboUvs, vboColors, texture;
};

#endif