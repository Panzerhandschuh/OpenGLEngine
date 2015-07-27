#ifndef MESH_H
#define MESH_H

#define GLEW_STATIC

#include <GL\glew.h>
#include "Image.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	void SetVertices(GLfloat verts[], int numVerts);
	void SetIndices(GLuint indices[], int numIndices);
	void SetTexCoords(GLfloat texCoords[], int numTexCoords);
	void SetTexture(const char* imgFile);
	void SetVertexColors(GLfloat colors[], int numColors);
	void Draw();
	void Draw(GLenum mode);

private:
	int numVerts, numTexCoords, numColors, numIndices;
	GLuint vao, vboVerts, vboTexCoords, vboColors, ebo, texture;
};

#endif