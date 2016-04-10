#include "BezierPath.h"

BezierPath::BezierPath(Model& sourceModel, BezierCurve& curve) : curve(curve)
{
	this->sourceModel = &sourceModel;
	pathModel.AddMesh();
	pointModel = new Model("Sphere.FBX");
}

BezierPath::~BezierPath()
{
	delete pointModel;
}

void BezierPath::Update(GLfloat deltaTime)
{
	if (selectedPoints.size() == 0)
	{
		if (InputManager::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
		{
			RaycastHit hit;
			if (RaycastPoints(hit, selectedPoints))
			{
				planePos = hit.point;
				useHeightPlane = InputManager::GetKey(GLFW_KEY_LEFT_CONTROL);
				pointOffsets.resize(selectedPoints.size());
				for (int i = 0; i < selectedPoints.size(); i++)
					pointOffsets[i] = *selectedPoints[i] - planePos;
				//cout << "dist: " << hit.distance << endl;
			}
		}
	}
	
	if (selectedPoints.size() > 0)
	{
		if (!InputManager::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
			selectedPoints.clear();
		else
		{
			vec3 rayPos = Camera::main.position;
			vec3 rayDir = Physics::GetRayFromMouse();
			vec3 planeNorm = (useHeightPlane) ? Camera::main.GetForward() : vec3(0.0f, 1.0f, 0.0f);
			RaycastHit hit;
			if (Physics::RaycastPlane(rayPos, rayDir, planePos, planeNorm, hit))
			{
				//cout << "dist: " << hit.distance << endl;
				if (useHeightPlane)
				{
					hit.point.x = planePos.x;
					hit.point.z = planePos.z;
				}

				for (int i = 0; i < selectedPoints.size(); i++)
					*selectedPoints[i] = hit.point + pointOffsets[i];
				DeformPath();
			}
		}
	}
}

void BezierPath::DeformPath()
{
	using namespace std;

	Mesh* sourceMesh = sourceModel->meshes[0];
	Bounds bounds(*sourceMesh);

	float meshLength = bounds.size[DEFORM_AXIS];
	float offset = -(bounds.center[DEFORM_AXIS] - (meshLength / 2));
	//cout << "Z-Axis Length: " << length << endl;
	//cout << "Z-Axis Offset: " << offset << endl;

	int numSegments = GetNumSegments();
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
			vec3 tang = normalize(curve.Derivative(percentLength));
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

void BezierPath::Draw(Shader& shader)
{
	shader.Use();
	mat4 model;
	shader.SetUniform("objectColor", 1.0f, 1.0f, 1.0f);
	shader.SetUniform("model", model);
	pathModel.Draw();

	// Draw bezier control points
	shader.SetUniform("objectColor", 0.0f, 1.0f, 0.0f);
	model = translate(mat4(1.0f), curve.p0);
	model = scale(model, vec3(2.0f));
	shader.SetUniform("model", model);
	pointModel->Draw();
	model = translate(mat4(1.0f), curve.p1);
	shader.SetUniform("model", model);
	//pointModel->Draw();
	model = translate(mat4(1.0f), curve.p2);
	shader.SetUniform("model", model);
	//pointModel->Draw();
	model = translate(mat4(1.0f), curve.p3);
	model = scale(model, vec3(2.0f));
	shader.SetUniform("model", model);
	pointModel->Draw();

	// Draw line segments
	//LineUtil::DrawLine(curve.p0, curve.p1);
	//LineUtil::DrawLine(curve.p1, curve.p2);
	//LineUtil::DrawLine(curve.p2, curve.p3);
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

bool BezierPath::RaycastPoints(RaycastHit& hit, std::vector<vec3*>& selectedPoints)
{
	vec3 rayPos = Camera::main.position;
	vec3 rayDir = Physics::GetRayFromMouse();
	//LineUtil::DrawRay(vec3(0.0f, 0.0f, 2.0f), rayDir * 5.0f);
	RaycastHit hit1, hit2, hit3, hit4;
	float minDist = std::numeric_limits<float>::max();
	vec3* controlPoint = 0;
	if (Physics::RaycastSphere(rayPos, rayDir, curve.p0, 1.0f, hit1))
	{
		std::cout << "hit sphere: " << hit1.point.x << ", " << hit1.point.y << ", " << hit1.point.z << std::endl;
		minDist = hit1.distance;
		hit = hit1;
		controlPoint = &curve.p0;
	}
	if (Physics::RaycastSphere(rayPos, rayDir, curve.p1, 1.0f, hit2))
	{
		std::cout << "hit sphere: " << hit2.point.x << ", " << hit2.point.y << ", " << hit2.point.z << std::endl;
		if (hit2.distance < minDist)
		{
			minDist = hit2.distance;
			hit = hit2;
			controlPoint = &curve.p1;
		}
	}
	if (Physics::RaycastSphere(rayPos, rayDir, curve.p2, 1.0f, hit3))
	{
		std::cout << "hit sphere: " << hit3.point.x << ", " << hit3.point.y << ", " << hit3.point.z << std::endl;
		if (hit3.distance < minDist)
		{
			minDist = hit3.distance;
			hit = hit3;
			controlPoint = &curve.p2;
		}
	}
	if (Physics::RaycastSphere(rayPos, rayDir, curve.p3, 1.0f, hit4))
	{
		std::cout << "hit sphere: " << hit4.point.x << ", " << hit4.point.y << ", " << hit4.point.z << std::endl;
		if (hit4.distance < minDist)
		{
			minDist = hit4.distance;
			hit = hit4;
			controlPoint = &curve.p3;
		}
	}

	selectedPoints.clear();
	if (controlPoint != 0)
	{
		selectedPoints.push_back(controlPoint);
		if (controlPoint == &curve.p0)
			selectedPoints.push_back(&curve.p1);
		else if (controlPoint == &curve.p3)
			selectedPoints.push_back(&curve.p2);

		return true;
	}

	return false;
}