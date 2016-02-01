#include "PathPoint.h"

using namespace std;
using namespace glm;

void PathPoint::Start()
{
	pathModel.AddMesh();
	pointModel = new Model("Sphere.FBX");

	SphereCollider* collider = entity->AddComponent<SphereCollider>();
	collider->radius = 1.0f;
}

PathPoint::~PathPoint()
{
	delete pointModel;
}

void PathPoint::Update(GLfloat deltaTime)
{
	if (SelectionManager::instance->selectedEnt == entity) // This point is selected
	{

	}
}

void PathPoint::Draw(Shader& shader)
{
	shader.Use();
	mat4 model;
	shader.SetUniform("objectColor", 1.0f, 1.0f, 1.0f);
	shader.SetUniform("model", model);
	pathModel.Draw();

	// Draw bezier control points
	vec3 color = (SelectionManager::instance->selectedEnt == entity) ? vec3(0.0f, 0.5f, 1.0f) : vec3(0.0f, 1.0f, 0.0f);
	shader.SetUniform("objectColor", color);
	model = translate(mat4(1.0f), transform->position);
	model = scale(model, vec3(2.0f));
	shader.SetUniform("model", model);
	pointModel->Draw();
}

void PathPoint::Init(Model& sourceModel, glm::vec3 pos)
{
	this->sourceModel = &sourceModel;
	transform->position = pos;
	startTang = transform->position + vec3(-5.0f, 0.0f, 0.0f);
	endTang = transform->position + vec3(5.0f, 0.0f, 0.0f);
}

void PathPoint::DeformPath()
{
	if (prev)
		prev->DeformPath();

	if (!next)
		return;

	Mesh* sourceMesh = sourceModel->meshes[0];
	Bounds bounds(*sourceMesh);
	BezierCurve curve(transform->position, endTang, next->startTang, next->transform->position);

	float meshLength = bounds.size[DEFORM_AXIS];
	float offset = -(bounds.center[DEFORM_AXIS] - (meshLength / 2));
	//cout << "Z-Axis Length: " << length << endl;
	//cout << "Z-Axis Offset: " << offset << endl;

	int numSegments = GetNumSegments(curve);
	//cout << "Num Segments: " << numSegments << endl;

	vector<vec3>& sourceVerts = sourceMesh->vertices;
	vector<vec3>& sourceNorms = sourceMesh->normals;
	vector<vec2>& sourceUvs = sourceMesh->uvs;
	vector<GLuint>& sourceIndices = sourceMesh->indices;
	vector<vec3> vertices(sourceVerts.size() * numSegments);
	vector<vec3> normals(sourceNorms.size() * numSegments);
	vector<vec2> uvs(sourceUvs.size() * numSegments);
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
			vec3 bPos = curve.GetPoint(percentLength);

			// Rotation
			vec3 tang = curve.GetTangent(percentLength);
			vec3 up(0.0f, 1.0f, 0.0f);
			vec3 biNormal = cross(up, tang);
			vec3 norm = cross(tang, biNormal);
			mat4 curveRot(biNormal.x, biNormal.y, biNormal.z, 0.0f,
				norm.x, norm.y, norm.z, 0.0f,
				tang.x, tang.y, tang.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

			// Get new index
			int currentIndex = i + (segment * sourceMesh->vertices.size());

			// Rotate normal
			normals[currentIndex] = (vec3)(curveRot * vec4(sourceNorms[i], 0.0f));

			// Position and rotate vertex
			vec3 vNew = sourceVerts[i]; // Create a temporary vector for rotating and positioning the transformed vertex
			vNew[DEFORM_AXIS] = 0.0f; // Set deform axis coordinate to 0 so the vertex is rotated relative to the curve
			vNew = (vec3)(curveRot * vec4(vNew, 0.0f));
			vNew += bPos;
			vertices[currentIndex] = vNew;
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

int PathPoint::GetNumSegments(BezierCurve& curve)
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