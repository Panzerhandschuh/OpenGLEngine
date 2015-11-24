#include "Model.h"

Model::Model() { }

Model::Model(string path)
{
	LoadModel(path);
}

Model::~Model()
{
	// Deallocate mesh objects
	for (int i = 0; i < meshes.size(); i++)
		delete meshes[i];
	meshes.clear();
}

void Model::Draw()
{
	for (int i = 0; i < meshes.size(); i++)
		meshes[i]->Draw();
}

void Model::LoadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "Assimp Error: " << importer.GetErrorString() << endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

Mesh* Model::AddMesh()
{
	Mesh* mesh = new Mesh;
	meshes.push_back(mesh);
	return mesh;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process node's meshes
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* mesh = ProcessMesh(aiMesh, scene);
		meshes.push_back(mesh);
	}

	// Process node's children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> uvs;
	vector<GLuint> indices;

	// Process vertex info
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		// Vertex
		glm::vec3 vert;
		aiVector3D aiVert = mesh->mVertices[i];
		vert.x = aiVert.x;
		vert.y = aiVert.y;
		vert.z = aiVert.z;
		vertices.push_back(vert);

		// Normal
		glm::vec3 normal;
		aiVector3D aiNormal = mesh->mNormals[i];
		normal.x = aiNormal.x;
		normal.y = aiNormal.y;
		normal.z = aiNormal.z;
		normals.push_back(normal);

		// Texture coordinate
		glm::vec2 uv;
		if (mesh->mTextureCoords[0]) // Check if mesh has channel 1 uv coordinates
		{
			aiVector3D aiUv = mesh->mTextureCoords[0][i];
			uv.x = aiUv.x;
			uv.y = aiUv.y;
		}
		else
			uv = glm::vec2(0.0f, 0.0f);
		uvs.push_back(uv);
	}

	// Process indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh;
	newMesh->SetVertices(vertices);
	newMesh->SetNormals(normals);
	newMesh->SetUvs(uvs);
	newMesh->SetIndices(indices);
	return newMesh;
}