#include "PathPointShapes.h"

using namespace std;
using namespace glm;

void PathPointShapes::Draw(Shader & shader)
{
	shader.Use();

	// Draw model
	mat4 model;
	shader.SetUniform("objectColor", 1.0f, 1.0f, 1.0f);
	shader.SetUniform("model", model);
	pathModel.Draw();

	PathPoint::Draw(shader);
}

void PathPointShapes::Init(vector<PathShape>& shapes, vec3 pos, vec3 dir)
{
	PathPoint::Init(pos, dir);

	pathModel.AddMesh();
	this->shapes = shapes;
}

void PathPointShapes::DeformPath()
{
	if (prev)
		prev->DeformPath();

	if (!next)
		return;

	PathShape pathShape = shapes[0];
	Shape shape = pathShape.shape;
	BezierCurve curve(transform->position, endHandle->position, next->startHandle->position, next->transform->position);

	int pathLoops = 24;
	int pathSegments = pathLoops - 1;
	int vertsInShape = shape.vertices.size();
	int indicesInShape = shape.indices.size();
	int numVerts = vertsInShape * pathLoops;
	int numIndices = 3 * indicesInShape * pathSegments;

	vector<vec3> vertices(numVerts);
	vector<vec3> normals(numVerts);
	vector<GLuint> indices(numIndices);

	quat startRot = angleAxis(angle, GetDirection());
	quat endRot = angleAxis(next->angle, next->GetDirection());

	//cout << startRot.x << " " << startRot.y << " " << startRot.z << " " << startRot.w << endl;
	//cout << endRot.x << " " << endRot.y << " " << endRot.z << " " << endRot.w << endl;

	LineUtil::DrawRay(curve.GetPoint(0.0f), QuaternionUtil::GetUp(startRot) * 5.0f, vec3(0.0f, 1.0f, 0.0f));
	LineUtil::DrawRay(curve.GetPoint(0.0f), QuaternionUtil::GetRight(startRot) * 5.0f, vec3(1.0f, 0.0f, 0.0f));
	LineUtil::DrawRay(curve.GetPoint(0.0f), QuaternionUtil::GetForward(startRot) * 5.0f, vec3(0.0f, 0.0f, 1.0f));
	
	LineUtil::DrawRay(curve.GetPoint(1.0f), QuaternionUtil::GetUp(endRot) * 5.0f, vec3(0.0f, 1.0f, 0.0f));
	LineUtil::DrawRay(curve.GetPoint(1.0f), QuaternionUtil::GetRight(endRot) * 5.0f, vec3(1.0f, 0.0f, 0.0f));
	LineUtil::DrawRay(curve.GetPoint(1.0f), QuaternionUtil::GetForward(endRot) * 5.0f, vec3(0.0f, 0.0f, 1.0f));

	for (int i = 0; i < pathLoops; i++)
	{
		// Get point and rotation on curve
		float t = (float)i / (float)(pathLoops - 1);
		vec3 point = curve.GetPoint(t);
		vec3 tang = curve.GetTangent(t);
		quat sRot = slerp(startRot, endRot, smoothstep(0.0f, 1.0f, t));

		vec3 up = QuaternionUtil::GetUp(sRot);
		vec3 biNormal = cross(up, tang);
		vec3 norm = cross(tang, biNormal);
		mat4 rotation = mat4(biNormal.x, biNormal.y, biNormal.z, 0.0f,
			norm.x, norm.y, norm.z, 0.0f,
			tang.x, tang.y, tang.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		int offset = i * vertsInShape;
		for (int j = 0; j < vertsInShape; j++)
		{
			int index = offset + j;
			vec2 vPos = shape.vertices[j] + pathShape.offset;
			vec3 pos = point + (vec3)(rotation * vec4(vPos, 0.0f, 0.0f));
			vec3 norm = (vec3)(rotation * vec4(shape.normals[j], 0.0f, 0.0f));
			vertices[index] = pos;
			normals[index] = norm;
		}
	}

	int tIndex = 0;
	for (int i = 0; i < pathSegments; i++)
	{
		int offset = i * vertsInShape;
		for (int j = 0; j < indicesInShape; j += 2)
		{
			//int tIndex = i * numLoopIndices; // Starting triangle index of this segment

			int a = offset + shape.indices[j] + vertsInShape;
			int b = offset + shape.indices[j];
			int c = offset + shape.indices[j + 1];
			int d = offset + shape.indices[j + 1] + vertsInShape;

			indices[tIndex] = a; tIndex++;
			indices[tIndex] = b; tIndex++;
			indices[tIndex] = c; tIndex++;
			indices[tIndex] = c; tIndex++;
			indices[tIndex] = d; tIndex++;
			indices[tIndex] = a; tIndex++;
		}
	}

	//for (int i = 0; i < vertices.size(); i++)
	//	LineUtil::DrawRay(vertices[i], normals[i]);

	Mesh* pathMesh = pathModel.meshes[0];
	pathMesh->SetVertices(vertices);
	pathMesh->SetNormals(normals);
	//pathMesh->SetUvs(uvs);
	pathMesh->SetIndices(indices);
}
