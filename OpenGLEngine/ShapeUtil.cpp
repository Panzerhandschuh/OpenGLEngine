#include "ShapeUtil.h"

using namespace std;
using namespace glm;

Shape ShapeUtil::CreateCircle(float radius, int numSides)
{
	Shape shape;
	vector<vec2> vertices(numSides);
	vector<vec2> normals(numSides);
	vector<float> us(numSides);
	vector<int> indices(numSides * 2);

	float theta = (two_pi<float>()) / (float)numSides;
	float cosine = cos(theta);
	float sine = sin(theta);

	float x = radius;
	float y = 0.0f;
	vec2 vert(x, y);
	vertices[0] = vert;
	normals[0] = normalize(vert);
	indices[0] = 0;
	indices[1] = 1;
	for (int i = 1; i < numSides; i++)
	{
		// Connect the vertices on the circle
		int index = i * 2;
		indices[index] = i;
		indices[index + 1] = (i + 1) % numSides;

		// Get point on circle (2D rotation matrix method)
		float t = x;
		x = cosine * x - sine * y;
		y = sine * t + cosine * y;

		vert = vec2(x, y);
		vertices[i] = vert;
		normals[i] = normalize(vert);
	}

	shape.vertices = vertices;
	shape.normals = normals;
	shape.indices = indices;
	return shape;
}
