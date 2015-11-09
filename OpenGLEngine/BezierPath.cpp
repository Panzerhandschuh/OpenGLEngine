#include "BezierPath.h"

BezierPath::BezierPath(Model& sourceModel, BezierCurve& curve) : curve(curve)
{
	this->sourceModel = &sourceModel;
}

void BezierPath::DeformPath()
{
	Mesh* sourceMesh = sourceModel->meshes[0];

	Bounds bounds(*sourceMesh);
	float length = bounds.size[DEFORM_AXIS];
	float offset = -(bounds.center[DEFORM_AXIS] - (length / 2));
	//std::cout << "Z-Axis Length: " << length << endl;
	//std::cout << "Z-Axis Offset: " << offset << endl;

	vector<glm::vec3> vertices = sourceMesh->vertices;
	vector<glm::vec3> normals = sourceMesh->normals;
	for (int i = 0; i < vertices.size(); i++)
	{
		float vPos = vertices[i][DEFORM_AXIS] + offset;
		float t = vPos / length; // Get normalized vertex coordinate in [0, 1] range

		// Position
		glm::vec3 bPos = curve.GetPoint(t);

		// Rotation
		glm::vec3 tang = curve.GetTangent(t);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::vec3 biNormal = glm::cross(up, tang);
		glm::vec3 norm = glm::cross(tang, biNormal);
		glm::mat4 curveRot(biNormal.x, biNormal.y, biNormal.z, 0.0f,
			norm.x, norm.y, norm.z, 0.0f,
			tang.x, tang.y, tang.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		// Rotate normal
		normals[i] = (glm::vec3)(curveRot * glm::vec4(normals[i], 0.0f));

		// Position and rotate vertex
		glm::vec3 vNew = vertices[i]; // Create a temporary vector for rotating and positioning the transformed vertex
		vNew[DEFORM_AXIS] = 0.0f; // Set deform axis coordinate to 0 so the vertex is rotated relative to the curve
		vNew = (glm::vec3)(curveRot * glm::vec4(vNew, 0.0f));
		vNew += bPos;
		vertices[i] = vNew;
	}

	Mesh* pathMesh = pathModel.AddMesh();
	pathMesh->SetVertices(vertices);
	pathMesh->SetNormals(normals);
	pathMesh->SetUvs(sourceMesh->uvs);
	pathMesh->SetIndices(sourceMesh->indices);
}

void BezierPath::Draw()
{
	pathModel.Draw();
}