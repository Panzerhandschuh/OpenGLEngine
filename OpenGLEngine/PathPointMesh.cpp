#include "PathPointMesh.h"

using namespace std;
using namespace glm;

void PathPointMesh::Draw(Shader& shader)
{
	shader.Use();

	// Draw model
	mat4 model;
	shader.SetUniform("objectColor", 1.0f, 1.0f, 1.0f);
	shader.SetUniform("model", model);
	pathModel.Draw();

	PathPoint::Draw(shader);
}

void PathPointMesh::Init(Model& sourceModel, vector<CrossSection>& crossSections, vec3 pos, vec3 dir)
{
	PathPoint::Init(pos, dir);

	pathModel.AddMesh();
	this->sourceModel = &sourceModel;
	this->crossSections = crossSections;
}

void PathPointMesh::DeformPath()
{
	if (prev)
		prev->DeformPath();

	if (!next)
		return;

	Mesh* sourceMesh = sourceModel->meshes[0];
	Bounds bounds(*sourceMesh);
	BezierCurve curve(transform->position, endHandle->position, next->startHandle->position, next->transform->position);

	float meshLength = bounds.size[DEFORM_AXIS];
	float offset = -(bounds.center[DEFORM_AXIS] - (meshLength / 2));
	//cout << "Z-Axis Length: " << length << endl;
	//cout << "Z-Axis Offset: " << offset << endl;

	float curveLength = curve.GetLength();
	int numSegments = GetNumSegments(curveLength);
	//cout << "Num Segments: " << numSegments << endl;

	vector<vec3>& sourceVerts = sourceMesh->vertices;
	vector<vec3>& sourceNorms = sourceMesh->normals;
	vector<vec2>& sourceUvs = sourceMesh->uvs;
	vector<GLuint>& sourceIndices = sourceMesh->indices;
	vector<vec3> vertices(sourceVerts.size() * numSegments);
	vector<vec3> normals(sourceNorms.size() * numSegments);
	vector<vec2> uvs(sourceUvs.size() * numSegments);
	vector<GLuint> indices(sourceIndices.size() * numSegments);

	vec3 point;
	mat4 rotation;
	quat startRot = angleAxis(angle, GetDirection());
	quat endRot = angleAxis(next->angle, next->GetDirection());

	//for (int i = 0; i < curve.arcLengths.size(); i++)
	//{
	//	float t = (float)i / (float)(curve.arcLengths.size() - 1);
	//	float tFixed = curve.GetLengthParameter(t);
	//	vec3 pos = curve.GetPoint(tFixed);
	//	LineUtil::DrawRay(pos, vec3(0.0f, 2.5f, 0.0f));
	//}

	// Deform the mesh using the bezier curve info
	for (int segment = 0; segment < numSegments; segment++)
	{
		// Generate indices for each path segment
		for (int i = 0; i < sourceIndices.size(); i++)
		{
			int currentIndex = i + (segment * sourceIndices.size());
			indices[currentIndex] = sourceIndices[i] + (segment * sourceVerts.size());
		}

		for (int i = 0; i < crossSections.size(); i++)
		{
			float tCrossSection = crossSections[i].tValue;
			float tNormalized = ((tCrossSection + segment) / numSegments);
			float tLength = curve.GetLengthParameter(tNormalized);
			float tValue = (InputManager::GetKey(GLFW_KEY_N)) ? tNormalized : tLength;
			//cout << "tNormalized:\t\t" << tNormalized << endl;
			//cout << "tLength:\t\t" << tLength << endl;

			if (segment == 0 || i != 0) // Don't recalc the point or rotation for the first cross section of a new segment since it uses the same values as the previous cross section
			{
				// Get position and rotation on the bezier curve for this cross section
				point = curve.GetPoint(tValue);
				vec3 tang = curve.GetTangent(tValue);
				quat sRot = slerp(startRot, endRot, smoothstep(0.0f, 1.0f, tValue));

				vec3 up = QuaternionUtil::GetUp(sRot);
				vec3 biNormal = cross(up, tang);
				vec3 norm = cross(tang, biNormal);
				rotation = mat4(biNormal.x, biNormal.y, biNormal.z, 0.0f,
					norm.x, norm.y, norm.z, 0.0f,
					tang.x, tang.y, tang.z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
			}

			vector<int>& indices = crossSections[i].indices;
			for (int j = 0; j < indices.size(); j++)
			{
				// Generate index from cross section
				int index = indices[j];
				int currentIndex = index + (segment * sourceVerts.size());

				// Rotate normal
				normals[currentIndex] = (vec3)(rotation * vec4(sourceNorms[index], 0.0f));

				// Position and rotate vertex
				vec3 vNew = sourceVerts[index]; // Create a temporary vector for rotating and positioning the transformed vertex
				vNew[DEFORM_AXIS] = 0.0f; // Set deform axis coordinate to 0 so the vertex is rotated relative to the curve
				vNew = point + (vec3)(rotation * vec4(vNew, 0.0f));
				vertices[currentIndex] = vNew;
			}
		}

		int offset = segment * sourceUvs.size();
		uvs.insert(uvs.begin() + offset, sourceUvs.begin(), sourceUvs.end());
	}

	Mesh* pathMesh = pathModel.meshes[0];
	pathMesh->SetVertices(vertices);
	pathMesh->SetNormals(normals);
	pathMesh->SetUvs(uvs);
	pathMesh->SetIndices(indices);
}

int PathPointMesh::GetNumSegments(float curveLength)
{
	Mesh* sourceMesh = sourceModel->meshes[0];
	Bounds bounds(*sourceMesh);

	float meshLength = bounds.size[DEFORM_AXIS];
	int numSegments = glm::max(1, (int)((curveLength / meshLength) + 0.5f));
	if (numSegments * sourceMesh->vertices.size() > MAX_PATH_VERTS)
		numSegments = MAX_PATH_VERTS / sourceMesh->vertices.size();

	return numSegments;
}