#include "BezierPath.h"

BezierPath::BezierPath(Model& sourceModel, BezierCurve& curve) : curve(curve)
{
	this->sourceModel = &sourceModel;
}

void BezierPath::DeformPath()
{
	Mesh* sourceMesh = sourceModel->meshes[0];
	Bounds bounds(*sourceMesh);

	float meshLength = bounds.size[DEFORM_AXIS];
	float offset = -(bounds.center[DEFORM_AXIS] - (meshLength / 2));
	//cout << "Z-Axis Length: " << length << endl;
	//cout << "Z-Axis Offset: " << offset << endl;

	float curveLength = curve.GetLength();
	int numSegments = GetNumSegments();
	//cout << "Num Segments: " << numSegments << endl;

	vector<glm::vec3> sourceVerts = sourceMesh->vertices;
	vector<glm::vec3> sourceNorms = sourceMesh->normals;
	vector<glm::vec2> sourceUvs = sourceMesh->uvs;
	vector<GLuint> sourceIndices = sourceMesh->indices;
	vector<glm::vec3> vertices(sourceVerts.size() * numSegments);
	vector<glm::vec3> normals(sourceNorms.size() * numSegments);
	vector<glm::vec2> uvs(sourceUvs.size() * numSegments);
	vector<GLuint> indices(sourceIndices.size() * numSegments);
	for (int segment = 0; segment < numSegments; segment++)
	{
		// Calculate index information
		for (int i = 0; i < sourceIndices.size(); i++)
		{
			int currentIndex = i + (segment * sourceIndices.size());
			indices[currentIndex] = sourceIndices[i] + (segment * sourceVerts.size());
		}

		// Calculate vertex information
		for (int i = 0; i < sourceVerts.size(); i++)
		{
			float vPos = sourceVerts[i][DEFORM_AXIS] + offset;
			float t = vPos / meshLength; // Get normalized vertex coordinate in [0, 1] range
			float percentLength = (t + segment) / numSegments;

			// Position
			glm::vec3 bPos = curve.GetPoint(percentLength);

			// Rotation
			glm::vec3 tang = curve.GetTangent(percentLength);
			glm::vec3 up(0.0f, 1.0f, 0.0f);
			glm::vec3 biNormal = glm::cross(up, tang);
			glm::vec3 norm = glm::cross(tang, biNormal);
			glm::mat4 curveRot(biNormal.x, biNormal.y, biNormal.z, 0.0f,
				norm.x, norm.y, norm.z, 0.0f,
				tang.x, tang.y, tang.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

			// Get new index
			int currentIndex = i + (segment * sourceMesh->vertices.size());

			// Rotate normal
			normals[currentIndex] = (glm::vec3)(curveRot * glm::vec4(sourceNorms[i], 0.0f));

			// Position and rotate vertex
			glm::vec3 vNew = sourceVerts[i]; // Create a temporary vector for rotating and positioning the transformed vertex
			vNew[DEFORM_AXIS] = 0.0f; // Set deform axis coordinate to 0 so the vertex is rotated relative to the curve
			vNew = (glm::vec3)(curveRot * glm::vec4(vNew, 0.0f));
			vNew += bPos;
			vertices[currentIndex] = vNew;
		}

		int offset = segment * sourceUvs.size();
		uvs.insert(uvs.begin() + offset, sourceUvs.begin(), sourceUvs.end());
	}

	Mesh* pathMesh = pathModel.AddMesh();
	pathMesh->SetVertices(vertices);
	pathMesh->SetNormals(normals);
	pathMesh->SetUvs(uvs);
	pathMesh->SetIndices(indices);
}

void BezierPath::Draw()
{
	pathModel.Draw();
}

int BezierPath::GetNumSegments()
{
	Mesh* sourceMesh = sourceModel->meshes[0];
	Bounds bounds(*sourceMesh);

	float meshLength = bounds.size[DEFORM_AXIS];
	float curveLength = curve.GetLength();
	int numSegments = glm::max(1, (int)((curveLength / meshLength) + 0.5f));
	if (numSegments * sourceMesh->vertices.size() > MAX_PATH_VERTS)
		numSegments = MAX_PATH_VERTS / sourceMesh->vertices.size();

	return numSegments;
}