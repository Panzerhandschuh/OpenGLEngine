#include "BezierMesh.h"

BezierMesh::BezierMesh(BezierCurve& curve)
{
	// Create bezier mesh
	const int numSegments = 20;
	const int numSlices = numSegments + 1;
	const int numVerts = 2 * numSlices;
	glm::vec3 bVerts[numVerts], bNorms[numVerts];
	for (int i = 0; i <= numSegments; i++)
	{
		float t = (float)i / (float)numSegments;
		glm::vec3 point = curve.GetPoint(t);
		glm::vec3 tang = glm::normalize(curve.GetTangent(t));
		glm::vec3 norm(0.0f, 1.0f, 0.0f);
		glm::vec3 biNorm = glm::cross(tang, norm);
		//std::cout << biNorm.x << ", " << biNorm.y << ", " << biNorm.z << std::endl;

		int index = i * 2; // Starting index of this slice
		bVerts[index] = point + biNorm;
		bVerts[index + 1] = point - biNorm;
		bNorms[index] = norm;
		bNorms[index + 1] = norm;
	}

	// Set up indices
	const int numIndices = 6 * numSegments;
	GLuint bIndices[numIndices];
	for (int i = 0; i < numSegments; i++)
	{
		int tIndex = i * 6; // Starting triangle index of this segment
		int vIndex = i * 2; // Starting vertex index of this segment

		bIndices[tIndex] = vIndex;
		bIndices[tIndex + 1] = vIndex + 1;
		bIndices[tIndex + 2] = vIndex + 2;

		bIndices[tIndex + 3] = vIndex + 1;
		bIndices[tIndex + 4] = vIndex + 3;
		bIndices[tIndex + 5] = vIndex + 2;
	}

	// Create triangle geometry
	mesh.SetVertices(bVerts, numVerts);
	mesh.SetIndices(bIndices, numIndices);
	mesh.SetNormals(bNorms, numVerts);
}