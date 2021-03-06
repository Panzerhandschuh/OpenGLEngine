#include "LineUtil.h"

using namespace glm;

void LineUtil::DrawRay(vec3 pos, vec3 dir, vec3 color)
{
	DrawLine(pos, pos + dir, color);
}

void LineUtil::DrawLine(vec3 v1, vec3 v2, vec3 color)
{
	vec3 verts[2];
	verts[0] = v1;
	verts[1] = v2;

	// Create buffers
	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Create shader
	Shader shader("line-vert.glsl", "line-frag.glsl");

	// Configure shader attributes
	glBindVertexArray(vao); // Bind vao

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind vbo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Configure vertex attribute
	glEnableVertexAttribArray(0); // Enable vertex attribute 0 to be used by vertex shader

	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3), verts, GL_STATIC_DRAW); // Set the vertices that vbo should use

	// Draw
	shader.Use();
	shader.SetUniform("view", Camera::main->GetView());
	shader.SetUniform("projection", Camera::main->proj);
	shader.SetUniform("lineColor", color);
	glDrawArrays(GL_LINES, 0, 2); // Draw using vertices

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo
	glBindVertexArray(0); // Unbind vao

	// Delete buffers
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}