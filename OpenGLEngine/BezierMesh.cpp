#include "BezierMesh.h"

BezierMesh::BezierMesh(BezierCurve& curve)
{
	// Create bezier mesh
	const int numSegments = 20;
	const int numSlices = numSegments + 1;
	const int numVerts = 2 * numSlices;
	vector<glm::vec3> verts(numVerts);
	vector<glm::vec3> norms(numVerts);
	vector<glm::vec2> uvs(numVerts);
	for (int i = 0; i <= numSegments; i++)
	{
		float t = (float)i / (float)numSegments;
		glm::vec3 point = curve.GetPoint(t);
		glm::vec3 tang = glm::normalize(curve.GetTangent(t));
		glm::vec3 norm(0.0f, 1.0f, 0.0f);
		glm::vec3 biNorm = glm::cross(tang, norm);
		//std::cout << biNorm.x << ", " << biNorm.y << ", " << biNorm.z << std::endl;

		int index = i * 2; // Starting index of this slice
		verts[index] = point + biNorm;
		verts[index + 1] = point - biNorm;
		norms[index] = norm;
		norms[index + 1] = norm;
		uvs[index] = glm::vec2(t, 0.0f);
		uvs[index + 1] = glm::vec2(t, 1.0f);
	}

	// Set up indices
	const int numIndices = 6 * numSegments;
	vector<GLuint> indices(numIndices);
	for (int i = 0; i < numSegments; i++)
	{
		int tIndex = i * 6; // Starting triangle index of this segment
		int vIndex = i * 2; // Starting vertex index of this segment

		indices[tIndex] = vIndex;
		indices[tIndex + 1] = vIndex + 1;
		indices[tIndex + 2] = vIndex + 2;

		indices[tIndex + 3] = vIndex + 1;
		indices[tIndex + 4] = vIndex + 3;
		indices[tIndex + 5] = vIndex + 2;
	}

	// Create triangle geometry
	mesh.SetVertices(verts);
	mesh.SetNormals(norms);
	mesh.SetUvs(uvs);
	mesh.SetIndices(indices);
}

void BezierMesh::Draw()
{
	mesh.Draw();
}