#ifndef MESH_H
#define MESH_H

#define GLEW_STATIC

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Image.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	void SetVertices(GLfloat verts[], int numVerts);
	void SetVertices(glm::vec3 verts[], int numVerts);
	void SetIndices(GLuint indices[], int numIndices);
	void SetNormals(GLfloat normals[], int numNormals);
	void SetNormals(glm::vec3 normals[], int numNormals);
	void SetTexCoords(GLfloat texCoords[], int numTexCoords);
	void SetTexture(const char* imgFile);
	void SetVertexColors(GLfloat colors[], int numColors);
	void Draw();
	void Draw(GLenum mode);

private:
	int numVerts, numIndices, numNormals, numTexCoords, numColors;
	GLuint vao, ebo, vboVerts, vboNormals, vboTexCoords, vboColors, texture;
};

#endif