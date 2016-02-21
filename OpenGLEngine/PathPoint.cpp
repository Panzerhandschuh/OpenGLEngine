#include "PathPoint.h"

using namespace std;
using namespace glm;

void PathPoint::Start()
{
	pathModel.AddMesh();
	pointModel = new Model("Sphere.FBX");

	SphereCollider* collider = entity->AddComponent<SphereCollider>();
	collider->radius = 1.0f;

	// Create start and end bezier handles
	Entity* start = EntityManager::CreateEntity();
	PathPointHandle* startHandle = start->AddComponent<PathPointHandle>();
	this->startHandle = start->transform;

	Entity* end = EntityManager::CreateEntity();
	PathPointHandle* endHandle = end->AddComponent<PathPointHandle>();
	this->endHandle = end->transform;

	startHandle->otherHandle = this->endHandle;
	endHandle->otherHandle = this->startHandle;
}

PathPoint::~PathPoint()
{
	delete pointModel;
}

void PathPoint::Update(GLfloat deltaTime)
{
	bool isSelected = (SelectionManager::instance->selectedEnt == entity);
	startHandle->entity->enabled = isSelected;
	endHandle->entity->enabled = isSelected;
}

void PathPoint::Draw(Shader& shader)
{
	shader.Use();

	// Draw model
	mat4 model;
	shader.SetUniform("objectColor", 1.0f, 1.0f, 1.0f);
	shader.SetUniform("model", model);
	pathModel.Draw();

	// Draw point
	bool isSelected = (SelectionManager::instance->selectedEnt == entity);
	vec3 color = (isSelected) ? vec3(0.0f, 0.5f, 1.0f) : vec3(0.0f, 1.0f, 0.0f);
	shader.SetUniform("objectColor", color);
	model = translate(mat4(1.0f), transform->position);
	model = scale(model, vec3(2.0f));
	shader.SetUniform("model", model);
	pointModel->Draw();

	if (isSelected)
	{
		// Draw tangent points
		bool isStartSelected = (SelectionManager::instance->selectedHandle == startHandle->entity);
		vec3 startcolor = (isStartSelected) ? vec3(1.0f, 1.0f, 0.0f) : vec3(1.0f, 1.0f, 1.0f);
		shader.SetUniform("objectColor", startcolor);
		model = translate(mat4(1.0f), startHandle->position);
		model = scale(model, vec3(1.0f));
		shader.SetUniform("model", model);
		pointModel->Draw();

		bool isEndSelected = (SelectionManager::instance->selectedHandle == endHandle->entity);
		vec3 endcolor = (isEndSelected) ? vec3(1.0f, 1.0f, 0.0f) : vec3(1.0f, 1.0f, 1.0f);
		shader.SetUniform("objectColor", endcolor);
		model = translate(mat4(1.0f), endHandle->position);
		shader.SetUniform("model", model);
		pointModel->Draw();

		// Draw tangent lines
		LineUtil::DrawLine(transform->position, startHandle->position);
		LineUtil::DrawLine(transform->position, endHandle->position);
	}
}

void PathPoint::Init(Model& sourceModel, vector<CrossSection>& crossSections, vec3 pos, vec3 dir)
{
	this->sourceModel = &sourceModel;
	this->crossSections = crossSections;
	transform->position = pos;
	startHandle->position = transform->position + (-5.0f * dir);
	endHandle->position = transform->position + (5.0f * dir);
}

void PathPoint::DeformPath()
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

	vec3 point;
	mat4 rotation;

	for (int segment = 0; segment < numSegments; segment++)
	{
		// Calculate index information
		for (int i = 0; i < sourceIndices.size(); i++)
		{
			int currentIndex = i + (segment * sourceIndices.size());
			indices[currentIndex] = sourceIndices[i] + (segment * sourceVerts.size());
		}

		for (int i = 0; i < crossSections.size(); i++)
		{
			float tValue = crossSections[i].tValue;
			float percentLength = ((tValue + segment) / numSegments);

			if (segment == 0 || i != 0) // Don't recalc the point or rotation for the first cross section of a new segment since it uses the same values as the previous cross section
			{
				point = curve.GetPoint(percentLength);

				vec3 tang = curve.GetTangent(percentLength);
				quat startRot = angleAxis(angle, GetDirection());
				quat endRot = angleAxis(next->angle, next->GetDirection());
				quat sRot = slerp(startRot, endRot, percentLength);
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
				// Get new index
				int index = indices[j];
				int currentIndex = index + (segment * sourceVerts.size());

				// Rotate normal
				normals[currentIndex] = (vec3)(rotation * vec4(sourceNorms[index], 0.0f));

				// Position and rotate vertex
				vec3 vNew = sourceVerts[index]; // Create a temporary vector for rotating and positioning the transformed vertex
				vNew[DEFORM_AXIS] = 0.0f; // Set deform axis coordinate to 0 so the vertex is rotated relative to the curve
				vNew = (vec3)(rotation * vec4(vNew, 0.0f));
				vNew += point;
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

void PathPoint::UpdateHandles(glm::vec3 moveDelta)
{
	startHandle->position += moveDelta;
	endHandle->position += moveDelta;
}

bool PathPoint::IsStartPoint()
{
	return (!prev);
}

bool PathPoint::IsEndPoint()
{
	return (!next);
}

bool PathPoint::IsIntermediatePoint()
{
	return (next && prev);
}

vec3 PathPoint::GetDirection()
{
	return normalize(endHandle->position - startHandle->position);
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